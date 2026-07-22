#pragma once
#include <Sexy/RtWeakPtr.h>
#include <Sexy/RtObject.h>

struct EntityCondition
{
    float m_startTime;
    float m_endTime;
    float m_eventTime;
    float m_additionalDataValue;
};

enum UnkConditionEnum
{
    b1 = 0,
    b2 = 1,
    b3 = 2,
    m_flatColorMode = 3,
    m_colorMixMode = 4,
    b6 = 5
};


template <typename T>

class EntityConditionTracker
{
public:
    std::vector<byte> m_conditionFlags;
    std::vector<EntityCondition> m_conditions;
    float m_speedScale;
    float m_dpsScale;
    float m_hpScale;
    float m_damageScale;
    char pad[4];
    std::vector<uint8_t> m_states;
    Sexy::Color m_shaderOverrideColor;
    Sexy::Color m_currentColor;
    Sexy::RtWeakPtr<T> m_owner;

    virtual ~EntityConditionTracker() {};
    virtual int GetConditionCount() {};
    virtual void Update() {};
};
