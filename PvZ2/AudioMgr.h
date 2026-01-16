#pragma once

class AudioMgr
{
public:
    static AudioMgr* GetInstance()
    {
        return *(AudioMgr**)getActualOffset(0x256A180);
    }

	static void PlayAudio(const SexyString& eventName)
	{
        CallFunc<void>(getActualOffset(0xA38D18), AudioMgr::GetInstance(), eventName, 0);
	}
};