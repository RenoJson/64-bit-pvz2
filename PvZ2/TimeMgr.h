#pragma once

class TimeMgr
{
public:
	char m_pad[36];
    float m_curTime;
    float m_unkTime; // this one is used in jetpack

    static TimeMgr* GetInstance()
    {
        return *(TimeMgr**)getActualOffset(0x256A010);
    }
};

static_assert(offsetof(TimeMgr, m_curTime) == 36);
static_assert(offsetof(TimeMgr, m_unkTime) == 40);