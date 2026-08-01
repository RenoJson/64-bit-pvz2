#pragma once
#include "BaseResource.h"

namespace Sexy
{
	class Image : public BaseResource
	{
	public:
		char pad[8];
		int mImageFlags;
		char pad1[4];
		void* mRenderData;
		SexyString mFilePath;
		int mWidth;
		int mHeight;

		// for image strips
		int mNumRows;
		int mNumCols;

		RtWeakPtr<Image> mAtlasImage;
		int mAtlasStartX;
		int mAtlasStartY;
		int mAtlasEndX;
		int mAtlasEndY;
	};

	static_assert(sizeof(Image) == 112);
	static_assert(offsetof(Image, mFilePath) == 48);
	static_assert(offsetof(Image, mAtlasStartY) == 100);
}