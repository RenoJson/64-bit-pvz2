#pragma once
#include "RtObject.h"
#include "RtWeakPtr.h"

namespace Sexy
{
	class BaseResource : public RtObject
	{
	public:
		char m_pad[4];
		RtWeakPtr<void> pamRtId;
		RtWeakPtr<void> unkPtr2;
	};
}