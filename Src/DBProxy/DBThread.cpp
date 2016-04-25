#include <assert.h>
#include <errno.h>
#include <sstream>

#include <boost/bind.hpp>

#include <Engine/Language/String.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Object/New.h>
#include <DBProxy/Util/DBProxyConfig.h>
#include <DBProxy/Util/DBUtil.h>
#include <DBProxy/DBThread.h>
#include <Engine/IPC/IPCHandle.h>
#include <Extern/Zlib/zlib.h>

using namespace std;

#define DEFAULT_COMPRESS_BUF_LEN    (4096)
#define SEGMENT_ALIGEN_SIZE         (4)
#define MAX_QUERY_MAIL_NUM          (50)
#define MAX_MAIL_LIST_NUM           (100)

DBThread::DBThread(int connID)
{
    this->connID = connID;      // 链接ID，用于分配处理消息
    msgFuncMap.clear();

    RegisterMsgFunction(SS_MSG_CREATE_ROLE_REQ, boost::bind(&DBThread::CreateRole, this, _1));
    RegisterMsgFunction(SS_MSG_QUERY_ROLE_REQ, boost::bind(&DBThread::QueryRole, this, _1));
    RegisterMsgFunction(SS_MSG_UPDATE_ROLE_REQ, boost::bind(&DBThread::UpdateRole, this, _1));
    RegisterMsgFunction(SS_MSG_APPEND_MAIL_REQ, boost::bind(&DBThread::AppendMail, this, _1));

    compressBuf = NewArray<char>(DEFAULT_COMPRESS_BUF_LEN);
    compressLen = DEFAULT_COMPRESS_BUF_LEN;
}

DBThread::~DBThread()
{
    DeleteArray(compressBuf);
}

bool DBThread::Initialize()
{
    const DBProxyConfigItem& dbProxyConfig = DBProxyConfigFile::Instance().Item();

    gameServerProcID = dbProxyConfig.GameServerProcID;
    driver = get_driver_instance();                                    // MySQL DB Driver

    if (!ConnectDB())
    {
        assert(false);
        return false;
    }
    
    continueFlag = true;                                               // 是否继续的标志
    
    msgList.clear();                                                   // 消息队列

    boost::function0<void> f = boost::bind(&DBThread::Execute, this);
    thread = boost::shared_ptr<boost::thread>(New<boost::thread>(f));   // Boost线程对象

    return true;
}

bool DBThread::ConnectDB()
{
    const DBProxyConfigItem& dbProxyConfig = DBProxyConfigFile::Instance().Item();
    
    try
    {    
        sql::ConnectOptionsMap connection_properties;
        connection_properties["hostName"]=dbProxyConfig.DBHost;
        connection_properties["userName"]=dbProxyConfig.DBUserName;
        connection_properties["password"]=dbProxyConfig.DBPassword;
        
        conn.reset(driver->connect(connection_properties));             // MySQL Connection
        conn->setAutoCommit(1);
        conn->setSchema(dbProxyConfig.DBName);

        newRoleSQL = "INSERT INTO player(account, data, mailbox) VALUE(?,?,?)";    // Insert SQL
        newRoleStmt.reset(conn->prepareStatement(newRoleSQL));          // Insert Prepared Statement

        getRoleSQL = "SELECT data,mailbox FROM player WHERE account=?";         // Get Role SQL
        getRoleStmt.reset(conn->prepareStatement(getRoleSQL));          // Get Role Prepared Statement

        updateRoleSQL = "UPDATE player SET data=?,mailbox=? WHERE account=?";     // Update Role SQL
        updateRoleStmt.reset(conn->prepareStatement(updateRoleSQL));    // Update Role Prepared Statement

        appendMailSQL = "INSERT INTO mail(account, content) VALUE(?,?)";// Insert SQL
        appendMailStmt.reset(conn->prepareStatement(appendMailSQL));    // Insert Prepared Statement

        getMailSQL = "SELECT seq,content FROM mail WHERE account=? ORDER BY seq DESC LIMIT ?";    // Get Mail SQL
        getMailStmt.reset(conn->prepareStatement(getMailSQL));          // Get Mail Prepared Statement

        delMailSQL = "DELETE FROM mail WHERE account=? AND seq<=?";      // Del Mail SQL
        delMailStmt.reset(conn->prepareStatement(delMailSQL));          // Del Mail Prepared Statement
    }
    catch(DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s", e.what(), e.getErrorCode(), e.getSQLStateCStr());
        return false;
    }

    return true;
}

bool DBThread::IsClosed()
{
    return conn->isClosed();
}

void DBThread::Execute()
{
    LOG_RUN("This is DBThread<%d>", connID);
    Update();
}

void DBThread::Stop()
{
    continueFlag = false;
    thread->join();
}
                        
void DBThread::InsertMsg(const MsgSet& msgSet)
{
    ThreadMutexGuard lock(dbMutex);
    msgList.push_back(msgSet);
}

void DBThread::Update()
{
    driver->threadInit();

    MsgSet msgSet;
    while(continueFlag)
    {
        if (IsClosed())
        {
            if (false == ConnectDB())
            {
                //assert(false);
                continue;
            }
        }
        
        bool get = false;

        msgSet.Clear();
        
        { // <- 这个花括号是必须的
            ThreadMutexGuard lock(dbMutex);
            DBMsgList::iterator iter = msgList.begin();
            if (iter != msgList.end())
            {
                msgSet = *iter;
                get = true;
                msgList.pop_front();
            }
        }

        if (get)
        {
            HandleDBMsg(msgSet);
        }

        Time::MsSleep(10);
    }
    driver->threadEnd();
}

void DBThread::HandleDBMsg(const MsgSet& msgSet)
{
    MsgID msgID = msgSet.msg().Get(0).id();
    MsgFunctionMap::iterator iter = msgFuncMap.find(msgID);
    if (iter == msgFuncMap.end())
    {
        LOG_TRACE("DB Msg <ID:%s> have no handler", MsgID_Name(msgID).c_str());
        return;
    }

    MsgFunction function = iter->second;
    function(msgSet);
}

//_newRoleSQL = "INSERT INTO player(account, data, mailbox) VALUE(?,?)";       // Insert SQL
void DBThread::CreateRole(const MsgSet& msgSet)
{
    const NetMsg& req = msgSet.msg().Get(0);
    assert(req.id() == SS_MSG_CREATE_ROLE_REQ);
    
    const SSCreateRoleReq& createRoleReq = req.body().ss_create_role_req();

    MsgSet rspMsgSet;
    rspMsgSet.mutable_head()->set_session_id(msgSet.head().session_id());
    rspMsgSet.mutable_head()->set_sequence(msgSet.head().sequence());

    NetMsg& rspMsg = *rspMsgSet.mutable_msg()->Add();
    rspMsg.set_id(SS_MSG_CREATE_ROLE_RSP);
    SSCreateRoleRsp* createRoleRsp = rspMsg.mutable_body()->mutable_ss_create_role_rsp();
    
    try
    {
        std::string compressStr;
        if (!Compress(createRoleReq.serialized_role_info(), compressStr))
        {
            createRoleRsp->set_successful(false);
            createRoleRsp->set_error_id(MSG_ERROR_UNDEFINE);
            SendMsg(rspMsgSet);
            return ;
        }

        BinaryBuff roleInfoBuff(compressStr);
        istream blobStream(&roleInfoBuff);

        BinaryBuff mailBoxBuff("");
        istream blobMailStream(&mailBoxBuff);

        newRoleStmt->setString(1, createRoleReq.account());
        newRoleStmt->setBlob(2, &blobStream);
        newRoleStmt->setBlob(3, &blobMailStream);

        int resNum = newRoleStmt->executeUpdate();

        if (1 == resNum)
        {
            createRoleRsp->set_successful(true);
        }
        else
        {
            createRoleRsp->set_successful(false);
            createRoleRsp->set_error_id(MSG_ERROR_NO_DATA);
        }

        SendMsg(rspMsgSet);
    }
    catch(DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s", 
                        e.what(), e.getErrorCode(), e.getSQLStateCStr());
        createRoleRsp->set_successful(false);
        createRoleRsp->set_error_id(MSG_ERROR_UNDEFINE);
        SendMsg(rspMsgSet);
    }
}


//_getRoleSQL = "SELECT data,mailbox FROM player WHERE account=?";   // Get Role SQL
void DBThread::QueryRole(const MsgSet& msgSet)
{
    const NetMsg& req = msgSet.msg().Get(0);
    assert(req.id() == SS_MSG_QUERY_ROLE_REQ);

    const SSQueryRoleReq& queryRoleReq = req.body().ss_query_role_req();

    MsgSet rspMsgSet;
    rspMsgSet.mutable_head()->set_session_id(msgSet.head().session_id());
    rspMsgSet.mutable_head()->set_sequence(msgSet.head().sequence());

    NetMsg& rspMsg = *rspMsgSet.mutable_msg()->Add();
    rspMsg.set_id(SS_MSG_QUERY_ROLE_RSP);
    SSQueryRoleRsp* queryRoleRsp = rspMsg.mutable_body()->mutable_ss_query_role_rsp();

    queryRoleRsp->set_account(queryRoleReq.account());

    try
    {
        getRoleStmt->setString(1, queryRoleReq.account());

        DBResultPtr res = getRoleStmt->executeQuery();

        if (res->rowsCount() == 0)
        {
            queryRoleRsp->set_query_successful(false);
            queryRoleRsp->set_error_id(MSG_ERROR_NO_DATA);
            SendMsg(rspMsgSet);
            return;
        }

        while (res->next())
        {
            istream* iStream = res->getBlob(1);
            std::string unCompressStr;
            if (!UnCompress(ReadString(*iStream), unCompressStr))
            {
                queryRoleRsp->set_query_successful(false);
                queryRoleRsp->set_error_id(MSG_ERROR_UNDEFINE);
                SendMsg(rspMsgSet);
                return;
            }

            queryRoleRsp->set_query_successful(true);
            queryRoleRsp->set_serialized_role_info(unCompressStr);

            istream* iMailStream = res->getBlob(2);
            std::string mailStr(ReadString(*iMailStream));

            std::string serializedMailStr;
            MergeMail(queryRoleReq.account(), mailStr, serializedMailStr);
            if (!serializedMailStr.empty())
            {
                queryRoleRsp->set_serialized_mail_info(serializedMailStr);
            }
            SendMsg(rspMsgSet);
        }
    }
    catch (DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s", 
                            e.what(), e.getErrorCode(), e.getSQLStateCStr());
        queryRoleRsp->set_query_successful(false);
        queryRoleRsp->set_error_id(MSG_ERROR_UNDEFINE);
        SendMsg(rspMsgSet);
    }
}

//_updateRoleSQL = "UPDATE player SET data=? WHERE account=?";        // Update Role SQL
void DBThread::UpdateRole(const MsgSet& msgSet)
{
    const NetMsg& req = msgSet.msg().Get(0);
    assert(req.id() == SS_MSG_UPDATE_ROLE_REQ);

    const SSUpdateRoleReq& updateRoleReq = req.body().ss_update_role_req();

    MsgSet rspMsgSet;
    rspMsgSet.mutable_head()->set_session_id(msgSet.head().session_id());
    rspMsgSet.mutable_head()->set_sequence(msgSet.head().sequence());

    NetMsg& rspMsg = *rspMsgSet.mutable_msg()->Add();
    rspMsg.set_id(SS_MSG_UPDATE_ROLE_RSP);

    SSUpdateRoleRsp* updateRoleRsp = rspMsg.mutable_body()->mutable_ss_update_role_rsp();
    updateRoleRsp->set_account(updateRoleReq.account());
    
    try
    {
        std::string compressStr;
        if (!Compress(updateRoleReq.serialized_role_info(), compressStr))
        {
            updateRoleRsp->set_update_successful(false);
            SendMsg(rspMsgSet);
            return;
        }

        BinaryBuff roleInfoBuff(compressStr);
        istream blobStream(&roleInfoBuff);
        updateRoleStmt->setBlob(1, &blobStream);

        std::string mailStr("");
        if (updateRoleReq.has_serialized_mail_info())
        {
            mailStr = updateRoleReq.serialized_mail_info();
        }
        BinaryBuff mailInfoBuff(mailStr);
        istream mailBobStream(&mailInfoBuff);
        updateRoleStmt->setBlob(2, &mailBobStream);

        updateRoleStmt->setString(3, updateRoleReq.account());

        updateRoleStmt->executeUpdate();
        updateRoleRsp->set_update_successful(true);
        
        SendMsg(rspMsgSet);
    }
    catch (DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s", 
                            e.what(), e.getErrorCode(), e.getSQLStateCStr());

        updateRoleRsp->set_update_successful(false);
        SendMsg(rspMsgSet);
    }
}

//_appendMailSQL = "INSERT INTO mail(account,content) VALUE(?,?)";        // Insert Mail SQL
void DBThread::AppendMail(const MsgSet& msgSet)
{
    const NetMsg& req = msgSet.msg().Get(0);
    assert(req.id() == SS_MSG_APPEND_MAIL_REQ);

    const SSAppendMailReq& appendMailReq = req.body().ss_append_mail_req();

    LOG_TRACE("Player[%s] append new email", appendMailReq.account().c_str());

    MsgSet rspMsgSet;
    rspMsgSet.mutable_head()->set_session_id(msgSet.head().session_id());
    rspMsgSet.mutable_head()->set_sequence(msgSet.head().sequence());

    NetMsg& rspMsg = *rspMsgSet.mutable_msg()->Add();
    rspMsg.set_id(SS_MSG_APPEND_MAIL_RSP);

    SSAppendMailRsp* appendMailRsp = rspMsg.mutable_body()->mutable_ss_append_mail_rsp();
    appendMailRsp->set_result(false);

    try
    {
        appendMailStmt->setString(1, appendMailReq.account());

        BinaryBuff mailInfoBuff(appendMailReq.content());
        istream mailBobStream(&mailInfoBuff);
        appendMailStmt->setBlob(2, &mailBobStream);

        appendMailStmt->executeUpdate();
        appendMailRsp->set_result(true);

        SendMsg(rspMsgSet);
    }
    catch (DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s",
                            e.what(), e.getErrorCode(), e.getSQLStateCStr());
        SendMsg(rspMsgSet);
    }
}

//_getMailSQL = "SELECT seq,content FROM mail WHERE account=? ORDER BY seq ASC LIMIT ?"  //Get Mail SQL
bool DBThread::QueryMail(const std::string& account, RepeatedPtrField<ComMailContent>& mailList, uint32_t& maxSeq)
{
    maxSeq = 0;
    try
    {
        getMailStmt->setString(1, account);
        getMailStmt->setInt(2, MAX_QUERY_MAIL_NUM);

        DBResultPtr res = getMailStmt->executeQuery();

        if (res->rowsCount() == 0)
        {
            return false;
        }

        ComMailContent tmpMail;
        while (res->next())
        {
            uint32_t seq = res->getInt(1);
            maxSeq = (maxSeq < seq) ? seq : maxSeq;

            istream* iStream = res->getBlob(2);
            if (!tmpMail.ParseFromString(ReadString(*iStream)))
            {
                assert(false);
                continue;
            }

            if (IsMailExpire(tmpMail))
            {
                continue;
            }

            ComMailContent& mail = *mailList.Add();
            mail.CopyFrom(tmpMail);
            mail.set_seq(seq);
        }
    }
    catch (DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s",
                            e.what(), e.getErrorCode(), e.getSQLStateCStr());
        return false;
    }

    return true;
}

bool DBThread::MergeMail(const std::string& account, const std::string& mailBinStr, std::string& mailStr)
{
    mailStr = mailBinStr;

    uint32_t maxSeq = 0;
    SSMailDBInfo mailDBInfo;
    RepeatedPtrField<ComMailContent >& repeatedMail = *mailDBInfo.mutable_mail();
    if (!QueryMail(account, repeatedMail, maxSeq))
    {
        LOG_TRACE("Player[%s] query mail fail, no mail data", account.c_str());
        return false;
    }

    if (!DelMail(account, maxSeq))
    {
        LOG_ERR("Player[%s] del mail fail", account.c_str());
        return false;
    }

    SSMailDBInfo tmpMailDBInfo;
    if (repeatedMail.size() < MAX_MAIL_LIST_NUM
            && !mailBinStr.empty()
            && tmpMailDBInfo.ParseFromString(mailBinStr))
    {
        for (int i = 0; i < tmpMailDBInfo.mail_size() && repeatedMail.size() <= MAX_MAIL_LIST_NUM; ++i)
        {
            const ComMailContent& mail = tmpMailDBInfo.mail(i);
            if (IsMailExpire(mail)
                    || mail.priority() == MAIL_PRIORITY_LOW
                    || (mail.priority() == MAIL_PRIORITY_NORMAL && mail.confirmed()))
            {
                time_t sendeTime = mail.send_time();
                LOG_TRACE("MergeMail account[%s] mail-theme[%s] mail-seq[%u] sender[%s] sendtime[%s]be filter",
                        account.c_str(), mail.theme().c_str(), mail.seq(), mail.sender().c_str(),
                        Time::DateTimeStr(&sendeTime));
                continue;
            }

            ComMailContent& newMail = *repeatedMail.Add();
            newMail.CopyFrom(mail);
        }
    }

    if (!mailDBInfo.SerializeToString(&mailStr))
    {
        LOG_ERR("Player[%s] serialized mail fail", account.c_str());
        mailStr = mailBinStr;
        return false;
    }

    return true;
}

//_delMailSQL = "DELETE FROM mail WHERE account=? AND seq<=?";      // Del Mail SQL
bool DBThread::DelMail(const std::string& account, uint32_t maxSeq)
{
    try
    {
        delMailStmt->setString(1, account);
        delMailStmt->setUInt(2, maxSeq);

        delMailStmt->executeUpdate();
        LOG_TRACE("try clear account[%s] mail that seq<=%u from mail table", account.c_str(), maxSeq);
    }
    catch (DBException& e)
    {
        LOG_ERR("SQLException: %s, error code: %d, SQLState: %s",
                            e.what(), e.getErrorCode(), e.getSQLStateCStr());
        return false;
    }
    return true;
}

void DBThread::SendMsg(MsgSet& msgSet)
{
    IPCHandle::Instance().SendMessage(gameServerProcID, msgSet);
}

void DBThread::RegisterMsgFunction(MsgID msgID, MsgFunction function)
{
    msgFuncMap.insert(MsgFunctionMap::value_type(msgID, function));
}

void DBThread::ExpandCompressBuf(int expectLen)
{
    if (expectLen <= compressLen)
    {
        return;
    }

    DeleteArray(compressBuf);

    compressLen = expectLen;
    if ((compressLen % SEGMENT_ALIGEN_SIZE) > 0)
    {
        compressLen = (expectLen/SEGMENT_ALIGEN_SIZE)*SEGMENT_ALIGEN_SIZE + SEGMENT_ALIGEN_SIZE;
    }

    compressBuf = NewArray<char>(compressLen);
}

bool DBThread::UnCompress(const std::string& compressStr, std::string& unCompressStr)
{
    ExpandCompressBuf(compressStr.size()*2.1);
    uLongf  unCompressLen = compressLen;
    int unCompressRet = uncompress((Bytef *)compressBuf, &unCompressLen, (const Bytef *)compressStr.c_str(), (uLong)compressStr.size());
    if (Z_OK != unCompressRet)
    {
        LOG_ERR("[UNCompress] uncompress fail, ret=%d", unCompressRet);
        return false;
    }

    unCompressStr.assign(compressBuf, unCompressLen);
    return true;
}

bool DBThread::Compress(const std::string& serializeStr, std::string& compressStr)
{
    ExpandCompressBuf(compressStr.size()*2.1);
    uLongf unCompressLen = compressLen;
    int compressRet = compress((Bytef *)compressBuf, &unCompressLen, (const Bytef *)serializeStr.c_str(), (uLong)serializeStr.size());
    if (Z_OK != compressRet)
    {
        LOG_ERR("[Compress] compress fail, ret=%d", compressRet);
        return false;
    }

    compressStr.assign(compressBuf, unCompressLen);
    return true;
}

bool DBThread::IsMailExpire(const ComMailContent& mail)
{
    ULONG expireTime = mail.expire_time();
    return (Time::GetCurSecond() >= expireTime);
}
