#pragma once
#include <Sexy/SexyApp.h>
#include <Sexy/ResourceManager.h>

class Board;
class WorldMap;

namespace Sexy {
	class ResourceManager;
}

class LawnApp
{
public:
	char pad1[244];
	int mWidth;
	int mHeight;
	char pad2[1046];
	int m_lawnAppScaledWidth;
	char pad3[514];
	int m_curWidth;
	int m_curHeight;
	char pad4[164];
	Sexy::ResourceManager* m_resourceManager;
	char pad5[472];
	Board* m_board;
	char pad6[8];
	float m_contentResolutionWidth;
	float m_contentResolutionHeight;
	char pad7[136];
	void* m_worldMap; //WorldMap*

	static LawnApp* GetInstance() {
		return *(LawnApp**)getActualOffset(0x2593760);
	};
};

static_assert(sizeof(LawnApp) == 2640);
static_assert(offsetof(LawnApp, mWidth) == 244);
static_assert(offsetof(LawnApp, mHeight) == 248);
//static_assert(offsetof(LawnApp, m_lawnAppScaledWidth) == 1308);
//static_assert(offsetof(LawnApp, m_curWidth) == 1348);
//static_assert(offsetof(LawnApp, m_curHeight) == 1352);
//static_assert(offsetof(LawnApp, m_resourceManager) == 1420);
//static_assert(offsetof(LawnApp, m_board) == 1728);
//static_assert(offsetof(LawnApp, m_contentResolutionWidth) == 1740);
//static_assert(offsetof(LawnApp, m_contentResolutionHeight) == 1744);
//static_assert(offsetof(LawnApp, m_worldMap) == 1884);