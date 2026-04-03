#pragma once

class StateMachineTableBuilder
{
public:
    static StateMachineTableBuilder* GetInstance()
    {
        return *(StateMachineTableBuilder**)getActualOffset(0x256A0E0);
    }
};
