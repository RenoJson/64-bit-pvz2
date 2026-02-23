#pragma once

class TimeMgr
{
public:
	char m_pad[36];
    float m_curTime;

    static TimeMgr* GetInstance()
    {
        return *(TimeMgr**)getActualOffset(0x256A010);
    }
};

static_assert(offsetof(TimeMgr, m_curTime) == 36);