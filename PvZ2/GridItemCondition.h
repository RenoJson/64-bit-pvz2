#pragma once
#include "EntityConditionTracker.h"


class GridItem;

enum GridItemConditions
{

};

class GridItemConditionTracker : public EntityConditionTracker<GridItem>
{
public:
	virtual int GetConditionCount() {}
	virtual void Update() {}
};