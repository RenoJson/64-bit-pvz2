#pragma once

namespace Sexy
{
	class SexyApp
	{
	public:
		char pad1[2432];
		float m_artScale;
		char pad2[4];
		float m_appScale;

		static Sexy::SexyApp* GetInstance() {
			return *(SexyApp**)getActualOffset(0x25A4618);
		};

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
		template<typename T = int>
		T ScaleRender(T objectScale)
		{
			return objectScale / (T)Sexy::SexyApp::GetInstance()->m_appScale;
		}
	};

	static_assert(sizeof(Sexy::SexyApp));
	static_assert(offsetof(Sexy::SexyApp, m_artScale) == 2432);
	static_assert(offsetof(Sexy::SexyApp, m_appScale) == 2440);
}