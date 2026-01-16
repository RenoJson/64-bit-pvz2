#pragma once

struct EntityCondition
{
    float m_startTime;
    float m_endTime;
    float m_eventTime;
    float m_additionalDataValue;
};

//template <typename T, typename CT>
class EntityConditionTracker
{
public:
    std::vector<uint8_t> m_conditionFlags;
    std::vector<EntityCondition> m_conditions;

public:
    virtual void Function0() {};
    virtual void Function1() {};
    virtual void Function2() {};
    virtual void Function3() {};
};

static_assert(sizeof(EntityConditionTracker) == 56);
static_assert(offsetof(EntityConditionTracker, m_conditionFlags) == 8);
static_assert(offsetof(EntityConditionTracker, m_conditions) == 32);

class ZombieConditionTracker : public EntityConditionTracker
{
public:
    //char m_pad[706];
};

//static_assert(sizeof(ZombieConditionTracker) == 764);