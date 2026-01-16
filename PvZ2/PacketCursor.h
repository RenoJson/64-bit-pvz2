#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>

class PacketCursor : public Sexy::RtObject
{
public:
	int mX;
	int mY;
	Sexy::Rect bounds;
	char pad001C[0xC];
	Sexy::RtWeakPtr<void> unkPtr;

	virtual void Function7() {};
	virtual void Function8() {};
	virtual void Function9() {};
	virtual void Function10() {};
	virtual void Function11() {};
	virtual bool CanPlantOnCursor() { return true; }; // stub implementation
	virtual void GetGridCoords() {}; // cannot accurately recreate atm i think
	virtual void AttemptPlant() {};
};