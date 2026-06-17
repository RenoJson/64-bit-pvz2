#pragma once
#include "RtObject.h"
#include "RtWeakPtr.h"

namespace Sexy
{
	class ResourceManager;

	class ResourceInfo : public RtObject
	{
	public:
		ResourceManager* mResourceMgr;
		char pad[4];
		RtWeakPtr<ResourceInfo> m_thisPtr;
		RtWeakPtr<void> m_resourceData;
		RtWeakPtr<void> m_referenceData;
		void* m_resGroup;
		const char* m_idCStr;
		const char* m_pathCStr;
		SexyString m_idStr;
		SexyString m_resPath;
		int m_artRes;
		int m_loc;
		bool m_fromProgram;
	};
}