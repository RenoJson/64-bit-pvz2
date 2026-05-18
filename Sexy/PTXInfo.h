#pragma once
#include <Sexy/IResStreamDriver.h>

struct PTXInfo {
	int width;
	int height;
	int unk;
	int format;
};

class AndroidDecodeTask
{
public:
	void* pad0008[2];
	Sexy::IResStreamDriver* resStreamDriver;
	char pad0032[8];
	int unkInt40;
	char pad0044[16];
	PTXInfo* ptxInfo;
	int rgbTextureID;
	int alphaTextureID;
	char pad0080[4];
	int unkInt84;
	char pad0088[8];
	unsigned char* pixels;

	virtual void Func0() {}; // just to force this class to have a vtable
};
static_assert(offsetof(AndroidDecodeTask, resStreamDriver) == 24);
static_assert(offsetof(AndroidDecodeTask, pad0044) == 44);
static_assert(offsetof(AndroidDecodeTask, ptxInfo) == 64);
static_assert(offsetof(AndroidDecodeTask, unkInt84) == 84);
static_assert(offsetof(AndroidDecodeTask, pixels) == 96);