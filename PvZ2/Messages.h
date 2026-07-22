#pragma once

enum MessageId
{
	PlantBoosted = 0xCF89B0,
	PressViewBoardOrZombies = 0xF65AC4,
	WaveManagerUpdateToPercent = 0x2ACD08,
	SetFlagCount = 0x56730C,
	DisableProgressMeter = -1,
	SurvivalLevelEnded = -2,
	NextWaveProgressMsg = 0x5672FC,
	ExplorerMsg = 0xB1D100,
	EntityTakeDamage = 0xC35748,
	LevelStartMsg = 0x2ACC78
};

inline void DisableProgressMeterMsg() {};
inline void SurvivalLevelEndedMsg() {};

class Messages
{
public:
	static uint64_t Get(MessageId id)
	{
		if ((uint64_t)id < 0)
		{
			switch (id)
			{
				case DisableProgressMeter:
					return (uint64_t)DisableProgressMeterMsg;
				case SurvivalLevelEnded:
					return (uint64_t)SurvivalLevelEndedMsg;
			}
		}
		return getActualOffset((uint64_t)id);
	}
};
