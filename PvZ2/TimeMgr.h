#pragma once

class TimeMgr
{
public:
	char m_pad[36];
    float m_curTime;

    static TimeMgr* GetInstance()
    {
        return *(TimeMgr**)getActualOffset(0x23774E8);
    }
};

static_assert(offsetof(TimeMgr, m_curTime) == 36);