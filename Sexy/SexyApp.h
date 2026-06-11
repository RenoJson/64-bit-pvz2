#pragma once

namespace Sexy
{
	class SexyApp
	{
	public:
		char pad1[1688];
		float m_artScale;
		char pad2[4];
		float m_appScale;

		static Sexy::SexyApp* GetInstance();

		template<typename T = int>
		T ScaleArt(T objectScale)
		{
			return (T)Sexy::SexyApp::GetInstance()->m_artScale * objectScale;
		}

		template<typename T = int>
		T UnscaleArt(T objectScale)
		{
			return objectScale / (T)Sexy::SexyApp::GetInstance()->m_artScale;
		}

		template<typename T = int>
		T ScaleToApp(T objectScale)
		{
			return (T)Sexy::SexyApp::GetInstance()->m_appScale * objectScale;
		}
	};

	static_assert(sizeof(Sexy::SexyApp));
	static_assert(offsetof(Sexy::SexyApp, m_artScale) == 1688);
	static_assert(offsetof(Sexy::SexyApp, m_appScale) == 1696);
}