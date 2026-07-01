#include "ResourceManager.h"

Sexy::RtWeakPtr<Sexy::PopAnim> Sexy::ResourceManager::GetPopAnimByName(const SexyString& popAnimName)
{
	return CallFunc<Sexy::RtWeakPtr<Sexy::PopAnim>, const SexyString*>(0x105CEF0, &popAnimName);
}
