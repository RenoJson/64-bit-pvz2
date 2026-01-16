#pragma once
#include "RSimpleType.h"

namespace Reflection
{
	class RField
	{
	public:
		char m_pad[44];
		SexyString m_fieldName;
		char m_pad2[12];
		uint m_fieldOffset;
		RSimpleType* m_fieldType;
	};

	//static_assert(sizeof(RField) == 168);
	//static_assert(offsetof(RField, m_fieldName) == 104);
	//static_assert(offsetof(RField, m_fieldOffset) == 104);
}