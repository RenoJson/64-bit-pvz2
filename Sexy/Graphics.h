#pragma once

namespace Sexy
{
	class Image;

	class Graphics
	{
	public:
		Image* mDestImage;
		float mTransX;
		float mTransY;
		float mScaleX;
		float mScaleY;
		float mScaleOrigX;
		float mScaleOrigY;
		Rect mClipRect;
		Color mColor;
		int mFont; // Sexy::Font type
		int mDrawMode;
		bool mColorizeImages;
		bool mFastStretch;
		bool mWriteColoredString;
		bool mLinearBlend;
		bool mIs3D;
		int mRenderDevice; // AndroidRenderDevice*
		int mRenderContext;
		int mGraphics3D;
		char mPad[58];
	};

	static_assert(sizeof(Graphics) == 152);
	static_assert(offsetof(Graphics, mTransX) == 8);
	static_assert(offsetof(Graphics, mClipRect) == 32);
	static_assert(offsetof(Graphics, mDrawMode) == 68);
	static_assert(offsetof(Graphics, mGraphics3D) == 88);
}