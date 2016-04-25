#include "ShmObject/SIShmAllocator.h"

SIShmAllocator::SIShmAllocator()
{
    shmManager = NULL;
    shmID      = -1;
}

SIShmAllocator::~SIShmAllocator()
{
}

bool SIShmAllocator::Initialize(int size)
{
    std::string name = get_current_dir_name();

    //printf("string::name=%s\n", name.c_str());

    Shmkey key(name.c_str(), 255);     // 255 - ftok的第二个参数的最大值

    try
    {
        shmManager = new ShmManager(boost::interprocess::open_or_create, key, size);
    }
    catch(ShmException &e)
    {
        printf("create shm failed!{key=%d, size=%d}\n", key.get_key(), size);
        return false;
    }
    
    shmID = shmManager->get_shmid();
    return true;
}

ShmSegmentManager* SIShmAllocator::GetShmSegment()
{
    return shmManager->get_segment_manager();
}

