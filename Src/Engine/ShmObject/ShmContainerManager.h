///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  共享内存容器管理器
//          edit by kcinz
//

#pragma once

#include <boost/function.hpp>
#include <list>
#include "Util/TMTSingleton.h"
#include "Util/Util.h"

class ShmContainerManager : public TMTSingleton<ShmContainerManager>
{
    typedef boost::function<void()>     RevertFunc;
    typedef std::list<RevertFunc>       RevertFuncList;
    
public:
                        ShmContainerManager();
                        ~ShmContainerManager();

    bool                Initialize();

    UINT                GetNewContainerID();

    void                RegisterRevertFunc(RevertFunc& func);

    void                RecoverID(UINT containerID);

    void                Revert();

    bool                NeedInit();

private:    
    UINT                maxID;
    RevertFuncList      listRevertFunc;

    bool                initFlag;
};

