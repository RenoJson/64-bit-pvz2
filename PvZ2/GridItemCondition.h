#pragma once
#include "EntityConditionTracker.h"


class GridItem;

enum GridItemConditions
{

};

class GridItemConditionTracker : public EntityConditionTracker<GridItem>
{
public:
	float m_scale;

	virtual int GetConditionCount() {}
	virtual void Update() {}
};