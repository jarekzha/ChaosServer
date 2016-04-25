#pragma once

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <list>
#include "Util/TMTSingleton.h"

typedef boost::function<void()> RevertFunc;

class IDPtrManager : public TMTSingleton<IDPtrManager>
{
public:
    IDPtrManager()  { listFunc.clear(); }
    ~IDPtrManager() { listFunc.clear(); }

    bool Initialize()   { initFlag = false; return true; }

    void RegisterIDPtrFunc(RevertFunc& func);

    void Revert();

    bool NeedInit()     {return initFlag;}

private:
    typedef std::list<RevertFunc>      FuncList;
    FuncList            listFunc;

    bool                initFlag;
};

