///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  使用共享内存的string
//          edit by Peter_ZHA
//

#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <sstream>
#include "ShmObject/SIShmAllocator.h"
#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtr.h"

class ShmString
{
    typedef boost::interprocess::allocator<char, ShmSegmentManager> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmStringType;

public:
                        ShmString()
    {
        ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
        if (shmContainerManager.NeedInit())   // 首次使用
        {
            stringID = shmContainerManager.GetNewContainerID();
        }
        else
        {
            shmContainerManager.RecoverID(stringID);
        }

        _FindOrConstruct();
    }

    virtual             ~ShmString()       
    {
        std::stringstream ss;
        ss << stringID;

        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        shmAllocator.Destroy<ShmStringType>(ss.str().c_str());
        
        stringID = 0;
    }

    ShmStringType*      operator->() const
    { return string; }
    
    const char*         operator=(const char* str) 
    {
        *string = str; 
        return string->c_str();
    }

    const char*         operator=(const std::string str)
    {
        *string = str.c_str();
        return string->c_str();
    }
                            
                        operator const char*() const
    { return string->c_str();}

private:
    void                _FindOrConstruct()
    {
        std::stringstream ss;
        ss << stringID;
        
        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        CharAllocator allot = CharAllocator(shmAllocator.GetShmSegment());
        string = shmAllocator.FindOrConstruct<ShmStringType>(ss.str().c_str())(allot);
        assert(string != NULL);
    }

private:
    UINT                stringID;
    ShmStringType*      string;
};


