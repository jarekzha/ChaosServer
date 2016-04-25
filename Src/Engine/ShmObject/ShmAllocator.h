#pragma once

#include <list>
#include <map>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Util/TMTSingleton.h"

#pragma pack(1)
struct ShmChunk
{
    unsigned int        size;           // chunk的大小
    unsigned char       type;           // chunk被使用的类型
    char                data[1];        // chunk数据的地址
};
#pragma pack()

const size_t ChunkHeadSize = sizeof(ShmChunk::size) + sizeof(ShmChunk::type);

/*
    ShmChunkPool: 共享内存块管理池
*/
class ShmChunkPool
{
    typedef std::list<ShmChunk*> ChunkList;
public:
                        ShmChunkPool();
                        ~ShmChunkPool();

    ShmChunk*           GetChunk();    
    void                InsertInto(ShmChunk* chunk);
    
private:
    ChunkList           unusedChunkList;
};

/*
    ShmAllocatorUnit: 共享内存分配器单元
    通过ftok使用路径生成和管理shm key，避免手动配置
*/
class ShmAllocatorUnit
{
    // size->chunkpool
    typedef std::map<unsigned int, ShmChunkPool*> ChunkPoolMap;   
public:
                        ShmAllocatorUnit();
                        ~ShmAllocatorUnit();

    bool                AttachShm(key_t key, unsigned int size);
    void*               Malloc(unsigned int size, unsigned char type);
    void                Free(void* data);
    void                LoadShmChunk(ShmChunkPool& chunkpool);
    void                RmShm();            // 删除共享内存

private:
    key_t               GetNextShmKey();
    void                SetNextShmKey(key_t nextKey);
    bool                IsHave(void* data);
    
    ShmChunk*           GetMemoryFromPool(unsigned int size);
    ShmChunk*           GetMemoryFromShm(unsigned int size);

private:
    ChunkPoolMap        chunkPoolMap;      // 共享内存池尺寸映射
    int                 shmID;             // 共享内存的ID
    
    void*               shmAddr;           // 共享内存地址
    unsigned int        shmSize;           // 共享内存大小
    unsigned int        freeOffset;        // 共享内存未使用地址的偏移量

    friend class ShmAllocator;
};

/*
    ShmAllocator: 共享内存分配器
    支持动态分配
*/
class ShmAllocator : public TMTSingleton<ShmAllocator>
{
    typedef std::list<ShmAllocatorUnit*> AllocatorList;
public:
                        ShmAllocator();
                        ShmAllocator(unsigned int allotSize);
                        ~ShmAllocator();

    void*               Malloc(unsigned int size, unsigned char type);
    void                Free(void* data);
    bool                LoadShmChunk(ShmChunkPool& chunkpool);
    void                RmShm();            // 删除共享内存

private:
    bool                Initialize();
    key_t               GetShmKey();
    key_t               GetFirstShmKey();
    key_t               GenerateNextShmKey();
    
private:
    AllocatorList       listAllocator;     // 分配器链表
    unsigned int        allotSize;         // 每次分配的大小
    int                 projID;           // 用于产生shm key的id
    bool                initedFlag;        // 是否reload shm的标志，在LoadSMChunk中使用
};

