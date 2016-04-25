
#pragma once

#include <list>
#include <map>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include <Protocol/NetMsg.pb.h>
#include <Engine/Thread/ThreadMutex.h>
#include <Engine/Util/ProtoBuf.h>
#include <Engine/Util/Util.h>

class DBThread
{    
    typedef sql::Driver* DBDriverPtr;
    typedef sql::SQLException DBException;
    typedef sql::ResultSet* DBResultPtr;
    
    typedef boost::shared_ptr<sql::Connection> DBConnPtr;
    typedef boost::shared_ptr<sql::PreparedStatement>   DBPrepStmt;
    typedef boost::shared_ptr<boost::thread>   ConnThread;
    typedef boost::function<void(const MsgSet&)>  MsgFunction;
    
    typedef std::list<MsgSet>       DBMsgList;
    typedef std::map<MsgID, MsgFunction>      MsgFunctionMap;
    
public:
                        DBThread(int connID);
                        ~DBThread();

    bool                Initialize();

    void                Execute();

    void                Stop();

    void                InsertMsg(const MsgSet& msgSet);

private:
    bool                ConnectDB();
    bool                IsClosed();
    void                HandleDBMsg(const MsgSet& msg);
    void                Update();

    void                CreateRole(const MsgSet& msgSet);
    void                QueryRole(const MsgSet& msgSet);
    void                UpdateRole(const MsgSet& msgSet);
    void                AppendMail(const MsgSet& msgSet);

    void                SendMsg(MsgSet& msgSet);

    void                RegisterMsgFunction(MsgID msgID, MsgFunction function);

    /*
     * @brief   更具应用实际大小扩展缓存
     *
     * @param expectLen 应用实际需要缓存大小(内部进行四字节对齐)
     *
     * */
    void                ExpandCompressBuf(int expectLen);

    /*
     * @brief   解压数据
     *
     * @param compressStr   带解压数据
     * @param unCompressStr 解压数据
     *
     * @return  [treu : success false : fail]
     *
     * */
    bool                UnCompress(const std::string& compressStr, std::string& unCompressStr);

    /*
     * @brief   压缩数据
     *
     * @param   serializeStr    待压缩数据
     * @param   compressStr     压缩后数据
     *
     * @return  [true : success false : fail]
     *
     * */
    bool                Compress(const std::string& serializeStr, std::string& compressStr);

    /*
     * @brief   读取邮件数据
     *
     * @param account       账户名
     * @param mailList      邮件数据
     * @param maxSeq        读取出邮件中最大seq值
     *
     * @return  [true : success false : fail]
     *
     * */
    bool                QueryMail(const std::string& account, RepeatedPtrField<ComMailContent>& mailList, uint32_t& maxSeq);

    /*
     * @brief           删除序号<=maxSeq的所有邮件
     *
     * @pram account    帐号
     * @param maxSeq    最大序号
     *
     * @return          [true : success false : fail]
     *
     * */
    bool                DelMail(const std::string& account, uint32_t maxSeq);

    /*
     * @brief   邮件是否过期
     *
     * @param mail  邮件内容
     *
     * @return  [true : expire false : not expire]
     *
     * */
    bool                IsMailExpire(const ComMailContent& mail);

    /*
     * @brief   合并收件箱和邮箱中邮件
     *
     * @param account   账户名
     * @param mailBinStr    当前收件箱邮件序列化数据
     * @param mailStr   合并后邮件序列化数据
     *
     * @return  [true : success false : fail]
     *
     * */
    bool                MergeMail(const std::string& account, const std::string& mailBinStr, std::string& mailStr);

private:
    ConnThread          thread;                 // Boost线程对象

    DBDriverPtr         driver;                 // MySQL DB Driver
    DBConnPtr           conn;                   // MySQL Connection

    std::string         newRoleSQL;             // New Role SQL
    DBPrepStmt          newRoleStmt;            // New Role Prepared Statement

    std::string         getRoleSQL;             // Get Role SQL
    DBPrepStmt          getRoleStmt;            // Get Role Prepared Statement

    std::string         updateRoleSQL;          // Update Role SQL
    DBPrepStmt          updateRoleStmt;         // Update Role Prepared Statement
    
    std::string         appendMailSQL;          // New Mail SQL
    DBPrepStmt          appendMailStmt;         // New Mail Prepared Statement

    std::string         getMailSQL;             // Get Mail SQL
    DBPrepStmt          getMailStmt;            // Get Mail Prepared Statement

    std::string         delMailSQL;             // Del Mail SQL
    DBPrepStmt          delMailStmt;            // Del Mail Prepared Statement

    bool                continueFlag;           // 是否继续的标志
    ThreadMutex         dbMutex;                // 消息队列互斥guard
    DBMsgList           msgList;                // 消息队列
    int                 connID;                 // 链接ID，用于分配处理消息

    int                 gameServerProcID;       // Game Server的proc id

    MsgFunctionMap      msgFuncMap;             // 消息函数映射表

    char*               compressBuf;            // 数据压缩缓冲区
    int                 compressLen;            // 数据压缩缓冲区长度
};

