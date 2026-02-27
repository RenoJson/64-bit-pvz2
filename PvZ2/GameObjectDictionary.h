#pragma once
#include "Sexy/RtObject.h"
#include "Sexy/RtWeakPtr.h"
#include <SexyTypes.h>
class GameObjectDictionary : public Sexy::RtObject {
public:

};
class AttachedGameObjectNode : public Sexy::RtObject {
public:
	SexyString m_name;
	Sexy::RtWeakPtr<Sexy::RtObject> m_object;
};
class AttachedEffect : public AttachedGameObjectNode {
public:
	int m_updateMethod; 
	char pad_2C[4];
	SexyString m_loopAnimName;
	Sexy::ValueRange m_loopDelayRange;
	float m_loopDelay; 
	float m_staticScale;
	float m_dynamicScale;
};
class AttachedEffectManager : public GameObjectDictionary {
public:
	std::vector<AttachedEffect> m_nodes;

	virtual void Function7() {};
	virtual uint Function8() {};
	virtual void* Function9() {};
	virtual void* Function10(int a2) {};
	virtual void* Function11(int a2) {};
	int GetObjectIndex(SexyString a2) 
	{
		typedef int (*getIndex)(AttachedEffectManager*, SexyString*);
		getIndex pGetIndex = (getIndex)getActualOffset(0x6629A8);
		return pGetIndex(this, &a2);
	};
	virtual void Function13(float duration) {};
};