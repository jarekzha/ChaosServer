#pragma once

#include "ShmObject/ShmObject.h"
#include "ShmObject/ShmObjectManager.h"
#include "ShmObject/IDPtrManager.h"

template <class T>
class IDPtr
{
    T*                  ptr;
    UINT                objectID;
    
public:
                        IDPtr()
    {
        IDPtrManager& idptrManager = IDPtrManager::Instance();
        if (idptrManager.NeedInit())
        {
           ptr = NULL;
           objectID = 0;
        }
        else
        {
            RegisterIDPtrRevert();
        }
    }

    /*用于构建临时对象*/
                        IDPtr(T* ptr)
    {
        this->ptr = ptr;
        objectID = (ptr == NULL)? 0 : ptr->GetObjectID();
    }

                        ~IDPtr() { ptr = NULL; objectID = 0; }

    const IDPtr&        operator=(const IDPtr& idPtr)
    {
        ptr = idPtr.ptr;
        objectID = idPtr.objectID;
        return *this;
    }

    T*                  Ptr() const { return ptr; }

    T*                  operator->() const { return ptr;}

    T&                  operator*() const { return *ptr;}

                        operator T*() const { return ptr;}

    T*                  operator=(T* ptr)
    {
        if (ptr != NULL)
        {
            assert(ptr->GetObjectID() != 0);
        }
        
        this->ptr = ptr;
        objectID = (ptr == NULL)? 0 : ptr->GetObjectID();
        return ptr;
    }

    void                Revert()
    {
        if (0 == objectID)
            return;
        
        ShmObject* objPtr = ShmObjectManager::Instance().FindShmObject(objectID);
        if (NULL == objPtr)
        {
            assert(false);
            return;
        }
        
        ptr = dynamic_cast<T*>(objPtr);
    }

    void                RegisterIDPtrRevert()
    {
        RevertFunc func;
        func = boost::bind(&IDPtr<T>::Revert, this);
        IDPtrManager& idptrManager = IDPtrManager::Instance();
        idptrManager.RegisterIDPtrFunc(func);
    }
};

