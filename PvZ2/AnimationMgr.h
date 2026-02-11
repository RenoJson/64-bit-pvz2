#pragma once
#include "GameObject.h"
#include "ZombieEvent.h"

class AnimationController : public GameObject
{
public:
	float m_startTime;
	float m_endTime;
	float m_duration;
	bool m_deleteOnEnd;
	SexyString m_description;
};

class MoveBoard : public AnimationController
{
public:
	int m_startXPos;
	int m_endXPos;
	int m_startYPos;
	int m_endYPos;
	int m_curveType;
};

class TimeEvent : public AnimationController {
public:
	int unk;
	ZombieEvent m_delegate;
};

class AnimationMgr : public GameObject
{
public:
	float m_time;
	bool m_paused;
	std::vector<Sexy::RtWeakPtr<AnimationController>> m_animControllers;
	std::vector<Sexy::RtWeakPtr<AnimationController>> m_animControllersToAdd;

	void AddAnimController(AnimationController* animController, float startTime, float& endTime) {
		typedef void (*addAnimController)(AnimationMgr*, AnimationController*, float, float*);
		addAnimController pAddAnimController = (addAnimController)getActualOffset(0x10328C0);

		return pAddAnimController(this, animController, startTime, &endTime);
	}

	virtual void Func13(int a2) {};
};

TimeEvent* MakeTimeEvent(Sexy::RtWeakPtr<GameObject>* owner, SexyString* eventName)
{
	typedef TimeEvent* (*makeTimeEvent)(Sexy::RtWeakPtr<GameObject>*, SexyString*);
	return ((makeTimeEvent)getActualOffset(0x334F34))(owner, eventName);
}

MoveBoard* MakeMoveBoard(float duration, int startX, int endX, int startY, int endY, int curveType)
{
	typedef MoveBoard* (*getMoveBoard)(float, int, int, int, int, int);
	return ((getMoveBoard)getActualOffset(0x334ED8))(2.5, startX, endX, startY, endY, 4);
}

void GetStartXAndEndX(int type, int& startX, int& endX)
{
	typedef void (*getStartAndEnd)(int, int*, int*);
	((getStartAndEnd)getActualOffset(0x671824))(type, &startX, &endX);
}