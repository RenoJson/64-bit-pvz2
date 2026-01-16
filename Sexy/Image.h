#pragma once
#include "BaseResource.h"

namespace Sexy
{
	class Image : public BaseResource
	{
	public:
		char m_pad2[12];
		SexyString mFilePath;
		int mWidth;
		int mHeight;

		// for image strips
		int mNumRows;
		int mNumCols;

		RtWeakPtr<Image> m_atlastImage;
		int m_atlastStartX;
		int m_atlastStartY;
		int m_atlastEndX;
		int m_atlastEndY;
	};

	static_assert(sizeof(Image) == 112);
	static_assert(offsetof(Image, mFilePath) == 48);
	static_assert(offsetof(Image, m_atlastStartY) == 100);
}