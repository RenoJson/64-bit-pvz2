#pragma once
#include "RtObject.h"
#include "RtWeakPtr.h"
#include "ResourceInfo.h"

namespace Sexy
{
	class ResourceInfo;

	class BaseResource : public RtObject
	{
	public:
		char pad1[4];
		RtWeakPtr<BaseResource> m_thisPtr;
		RtWeakPtr<ResourceInfo> m_resInfo;
		char pad2[4];
	};
}