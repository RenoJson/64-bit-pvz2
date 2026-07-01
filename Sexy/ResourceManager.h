#pragma once
#include <Sexy/RtWeakPtr.h>

namespace Sexy
{
	class PopAnim;

	class ResourceManager
	{
	public:

		virtual void Function0() {};
		virtual void Function1() {};
		virtual void Function2() {};
		static Sexy::RtWeakPtr<Sexy::PopAnim> GetPopAnimByName(const SexyString& popAnimName);
	};
}