#pragma once
#include <Sexy/LazySingleton.h>

class AudioMgr 
{
public:
    static AudioMgr* GetInstance()
    {
        return *(AudioMgr**)getActualOffset(0x256A180);
    }

    static int64_t PostEvent(const SexyString& eventName)
    {
        return CallFunc<int64_t, AudioMgr*, const char*, int>(0xA38D18, AudioMgr::GetInstance(), eventName.c_str(), 0);
    }
};