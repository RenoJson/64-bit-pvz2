#include "Effect_PopAnim.h"
#include "Sexy/ResourceManager.h"

void Effect_PopAnim::PlayAnimation(const SexyString& animName, int selectionMethod)
{
	return CallFunc<void, Effect_PopAnim*, const SexyString*, int>(0x1232824, this, &animName, selectionMethod);
}

void Effect_PopAnim::SetFromResource(const SexyString& resourceName)
{
	auto resPtr = Sexy::ResourceManager::GetPopAnimByName(resourceName);
	this->SetFromResource(resPtr.Get());
}

void Effect_PopAnim::SetFromResource(void* popAnimRes, Sexy::RtClass* rtClass)
{
	return CallFunc<void, Effect_PopAnim*, void*, Sexy::RtClass*>(0x123251C, this, popAnimRes, rtClass);
}

int Effect_PopAnim::SetFocusFracAndScale(int unkValue)
{
	return CallFunc<int, Effect_PopAnim*, int>(0x12325F8, this, unkValue);
}

void Effect_PopAnim::SetPositionAndLayer(const Sexy::SexyVector3& pos, int layer)
{
	return CallFunc<void, Effect_PopAnim*, const Sexy::SexyVector3*, int>(0xAC46DC, this, &pos, layer);
}
