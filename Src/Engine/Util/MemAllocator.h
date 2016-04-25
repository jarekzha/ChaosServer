// 内存分配器接口
#pragma once

class MemAllocator
{
public:
    virtual             ~MemAllocator(){}

    virtual void*       Malloc(unsigned int size, unsigned short type) = 0;
    
    virtual void        Free(void* data) = 0;
};


