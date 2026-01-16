#pragma once
#include <Sexy/RtObject.h>

class WorldMap : public Sexy::RtObject
{
public:
    char m_pad[496];
    float m_posX;
    float m_posY;
    int m_boundaryX;
    int m_boundaryY;
    int m_boundaryWidth;
    int m_boundaryHeight;
    char m_pad2[256];
    float m_float1;
    float m_float2;

    //static WorldMap* GetWorldMap()
    //{
    //    
    //}
};

static_assert(offsetof(WorldMap, m_posX) == 504);
static_assert(offsetof(WorldMap, m_posY) == 508);
static_assert(offsetof(WorldMap, m_boundaryX) == 512);
static_assert(offsetof(WorldMap, m_float1) == 784);