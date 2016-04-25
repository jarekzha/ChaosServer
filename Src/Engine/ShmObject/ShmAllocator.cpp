#include <errno.h>
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "ShmObject/ShmAllocator.h"

ShmChunkPool::ShmChunkPool()
{
    unusedChunkList.clear();
}

ShmChunkPool::~ShmChunkPool()
{
    unusedChunkList.clear();
}

ShmChunk* ShmChunkPool::GetChunk()
{
    if (unusedChunkList.empty())
    {
        return NULL;
    }
    
    ChunkList::iterator iter = unusedChunkList.begin();   
    ShmChunk* chunk = *iter;
    unusedChunkList.pop_front();
    
    return chunk;
}

void ShmChunkPool::InsertInto(ShmChunk* chunk)
{
    unusedChunkList.push_back(chunk);
}

/*****************************************************************************/
ShmAllocatorUnit::ShmAllocatorUnit()
{
    chunkPoolMap.clear();
    shmAddr = NULL;
    shmSize = 0;
    freeOffset = 0;
}
        
ShmAllocatorUnit::~ShmAllocatorUnit()
{
    if (shmAddr)
    {
        shmdt(shmAddr);
        shmAddr = NULL;
    }

    ChunkPoolMap::iterator iter = chunkPoolMap.begin();
    for (; iter != chunkPoolMap.end(); ++iter)
    {
        ShmChunkPool* chunkPool = iter->second;
        delete chunkPool;
    }

    chunkPoolMap.clear();

    shmSize = 0;
    freeOffset = 0;
}

bool ShmAllocatorUnit::AttachShm(key_t key, unsigned int size)
{
    bool newShm = true;        // 是否创建
    
    shmID = shmget(key, size, IPC_CREAT|IPC_EXCL|0666);

    if (shmID < 0)
    {
        if (errno == EEXIST)
        {
            // 已存在
            shmID = shmget(key, size, 0666);
            if (shmID < 0)
            {
                return false;
            }

            newShm = false;     // 已经存在
        }
        else
        {
            return false;
        }
    }

    shmAddr = shmat(shmID, NULL, 0);
    if (NULL == shmAddr || (long)(shmAddr) == -1)
    {
        return false;
    }

    shmSize = size;
    freeOffset = sizeof(key_t);            // 每个shm的头用来保存下个shm的key

    if (newShm)
    {
        SetNextShmKey(0);                   // 初始化下个shm的key为0
    }
    
    return true;
}

void* ShmAllocatorUnit::Malloc(unsigned int size, unsigned char type)
{
    // 1-Get Memory from map
    ShmChunk* chunk = GetMemoryFromPool(size);
    if (chunk)
    {
        chunk->type = type;
        return (void*)chunk->data;
    }
    
    // 2-Get Memory from exist shm
    chunk = GetMemoryFromShm(size);
    if (chunk)
    {
        chunk->type = type;
        return (void*)chunk->data;
    }    

    return NULL;
}

void ShmAllocatorUnit::Free(void* data)
{
    ShmChunk* chunk = (ShmChunk*)((char*)data - ChunkHeadSize);

    chunk->type = 0;
    ChunkPoolMap::iterator iter = chunkPoolMap.find(chunk->size);
    if (iter != chunkPoolMap.end())
    {
        ShmChunkPool* chunkPool = iter->second;
        chunkPool->InsertInto(chunk);
    }
    else
    {
        ShmChunkPool* chunkPool = new ShmChunkPool();
        chunkPool->InsertInto(chunk);

        chunkPoolMap.insert(ChunkPoolMap::value_type(chunk->size, chunkPool));
    }
}

bool ShmAllocatorUnit::IsHave(void* data)
{
    ShmChunk* chunk = (ShmChunk*)((char*)data - ChunkHeadSize);
    
    if (chunk < shmAddr)
    {
        return false;
    }

    if (((char*)chunk + chunk->size) > ((char*)shmAddr + shmSize))
    {
        return false;
    }

    return true;
}

void ShmAllocatorUnit::LoadShmChunk(ShmChunkPool& chunkpool)
{
    while(freeOffset < shmSize)
    {
        ShmChunk* chunk = (ShmChunk*)((char*)shmAddr + freeOffset);

        if (0 == chunk->size)
            break;

        if (chunk->type != 0)
        {
            chunkpool.InsertInto(chunk);
        }
        else
        {
            Free(chunk->data);
        }
        
        freeOffset += (ChunkHeadSize + chunk->size);
    }
}


key_t ShmAllocatorUnit::GetNextShmKey()
{
    key_t* key = (key_t*)shmAddr;

    return *key;
}

void ShmAllocatorUnit::SetNextShmKey(key_t nextKey)
{
    *(key_t*)shmAddr = nextKey;
}

ShmChunk* ShmAllocatorUnit::GetMemoryFromPool(unsigned int size)
{
    ChunkPoolMap::iterator iter = chunkPoolMap.find(size);
    if (iter == chunkPoolMap.end())
    {
        return NULL;
    }

    ShmChunkPool* chunkPool = iter->second;
    ShmChunk* chunk = chunkPool->GetChunk();
    if (NULL == chunk)
    {
        return NULL;
    }
    
    chunk->type = 0;
        
    return chunk;
}
        
ShmChunk* ShmAllocatorUnit::GetMemoryFromShm(unsigned int size)
{
    unsigned int chunkSize = size + ChunkHeadSize;
    if (chunkSize > (shmSize - freeOffset))
    {
        // no enough memory
        return NULL;
    }

    ShmChunk* chunk = (ShmChunk*)((char*)shmAddr + freeOffset);

    if(chunk->size != 0)
    {
        assert(false);
    }

    chunk->size = size;
    chunk->type = 0;
    
    freeOffset += chunkSize;

    return chunk;
}

void ShmAllocatorUnit::RmShm()
{
    shmctl(shmID, IPC_RMID, NULL);
}

/************************************************************************/
ShmAllocator::ShmAllocator()
{
    listAllocator.clear();
    allotSize = 209715200;     // 默认200MB，1G就需要5个sm_alloctor
    projID = 1;
    initedFlag = false;
}

ShmAllocator::ShmAllocator(unsigned int allotSize)
{
    listAllocator.clear();
    this->allotSize = allotSize;     // 用使用者定义单个sm_alloctor的大小
    projID = 0;
    initedFlag = false;
}

ShmAllocator::~ShmAllocator()
{
    AllocatorList::iterator iter = listAllocator.begin();
    for (; iter != listAllocator.end(); ++iter)
    {
        ShmAllocatorUnit* smAllot = *iter;
        delete smAllot;
    }

    listAllocator.clear();

    initedFlag = false;
    allotSize = 0;
    projID = 0;
}

bool ShmAllocator::Initialize()
{    
    key_t key = GetFirstShmKey();

    while(1)
    {            
        ShmAllocatorUnit* smAllot = new ShmAllocatorUnit();
    
        bool ret = smAllot->AttachShm(key, allotSize);
        if (!ret)
        {
            return false;
        }

        listAllocator.push_back(smAllot);

        key = smAllot->GetNextShmKey();
        if (0 == key)
        {
            break;
        }

        ++projID;             // 必须跟随key数量的变化而变化
    }

    initedFlag = true;         // 标识shm已经做了reload

    return true;
}

void* ShmAllocator::Malloc(unsigned int size, unsigned char type)
{
    void* ptr = NULL;

    if (!initedFlag)
    {
        if (!Initialize())
        {
            return NULL;
        }
    }

    AllocatorList::iterator iter = listAllocator.begin();
    for(; iter != listAllocator.end(); ++iter)
    {
        ShmAllocatorUnit* smAllot = *iter;
        ptr = smAllot->Malloc(size, type);
        if (ptr)
        {
            return ptr;
        }

        continue;
    }

    // 创建新的allocator
    key_t key = GetShmKey();
    if (key < 0)
    {
        return NULL;
    }
    
    ShmAllocatorUnit* smAllot = new ShmAllocatorUnit();
    if (!smAllot->AttachShm(key, allotSize))
    {
        return NULL;
    }

    if (!listAllocator.empty())
    {
        ShmAllocatorUnit* preSMAllot = listAllocator.back();
        preSMAllot->SetNextShmKey(key);
    }
    
    listAllocator.push_back(smAllot);
    
    ptr = smAllot->Malloc(size, type);

    return ptr;
}

void ShmAllocator::Free(void* data)
{
    if (!initedFlag)
    {
        if (!Initialize())
        {
            return;
        }
    }
    
    AllocatorList::iterator iter = listAllocator.begin();
    for(; iter != listAllocator.end(); ++iter)
    {
        ShmAllocatorUnit* smAllot = *iter;
        if (smAllot->IsHave(data))
        {
            smAllot->Free(data);
            break;
        }
    }
}

bool ShmAllocator::LoadShmChunk(ShmChunkPool& chunkpool)
{
    if (!initedFlag)
    {
        if (!Initialize())
        {
            return false;
        }
    }
    
    AllocatorList::iterator iter = listAllocator.begin();
    for(; iter != listAllocator.end(); ++iter)
    {
        ShmAllocatorUnit* smAllot = *iter;
        smAllot->LoadShmChunk(chunkpool);
    }

    return true;
}

key_t ShmAllocator::GetShmKey()
{
    key_t key = 0;   
    if (listAllocator.empty())
    {
        key = GetFirstShmKey();   
    }
    else
    {
        key = GenerateNextShmKey();        
    }

    return key;
}

key_t ShmAllocator::GetFirstShmKey()
{
    key_t key = 0;

    // step 1: 先读取本地记录的shm id，防止ftok因为路径或者文件变化生成key变化
    /*
    std::ifstream ifs("shm.id");
    if (ifs)
    {
        ifs >> key;
        ++_proj_id;
        return key;
    }
    */

    // step 2: 如果step 1失败则根据ftok来生成shm key
    std::string path;
    path = get_current_dir_name();

    key = ftok(path.c_str(), ++projID);
    if (key < 0)
    {
        return -1;
    }

    /* 暂时关闭写shm.id，如有必要再打开，只要目录(不带文件)不变，可以不用生成该文件
    std::ofstream of("shm.id");

    of << key;

    of.flush();
    of.close();
    */

    return key;
}

key_t ShmAllocator::GenerateNextShmKey()
{
    std::string path;
    path = get_current_dir_name();

    int nextID = ++projID;

    if (nextID > 255 || nextID <=0 )
        return -1;          // 只要小于0

    key_t key = ftok(path.c_str(), nextID);
    return key;
}

void ShmAllocator::RmShm()
{    
    if (!initedFlag)
    {
        if (!Initialize())
        {
            return;
        }
    }
    
    AllocatorList::iterator iter = listAllocator.begin();
    for(; iter != listAllocator.end(); ++iter)
    {
        ShmAllocatorUnit* smAllot = *iter;
        smAllot->RmShm();
        delete smAllot;
    }

    listAllocator.clear();

    projID = 0;
}

