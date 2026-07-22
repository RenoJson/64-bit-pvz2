#include "ZombieConditions.h"

void ZombieConditionTracker::UpdateIfB1()
{
    if (this->m_states[b1])
    {
        this->Update();
        this->m_states[b1] = false;
    }
}