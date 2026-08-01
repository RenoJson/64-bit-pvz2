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
		RtWeakPtr<BaseResource> m_thisPtr;
		RtWeakPtr<ResourceInfo> m_resInfo;
	};
}