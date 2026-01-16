#pragma once
#include "BaseResource.h"

namespace Sexy
{
	class SexyMatrix3 {
	public:
		float mMat[3][3];
	};

	class SexyTransform2D : public SexyMatrix3 {
	public:
		//bool mComplex;
		//bool mHaveRot;
		//bool mHaveScale;
		//float mTransX1;
		//float mTransY1;
		//float mTransX2;
		//float mTransY2;
		//float mScaleX;
		//float mScaleY;
		//float mRot;

		SexyTransform2D() {
			memset(mMat, 0, sizeof(mMat));
			mMat[0][0] = 1.0;
			mMat[1][1] = 1.0;
			mMat[2][2] = 1.0;
		}
	};

	class PopAnim : public BaseResource
	{
	public:
		char mPad2[8];
		int mDrawScale;
		int mImgScale;
		int mAnimRate;
		Rect mAnimRect;
		int mLastPlayedFrameLabel;
		char mPad3[8];
		int mUnkStruct;
		int mMainAnimDef;
		int mBlendTicksTotal;
		int mBlendTicksCur;
		int mBlendDelay;
		SexyTransform2D mTransform;
		Color mColor;
		bool mAdditive;
		char mPad4[4];
		bool mAnimRunning;
		bool mPaused;
		bool mColorizeType;
	};

	static_assert(sizeof(PopAnim) == 160);
	//static_assert(offsetof(PopAnim, mDrawScale) == 40);
	//static_assert(offsetof(PopAnim, mAnimRect) == 52);
	//static_assert(offsetof(PopAnim, mTransform) == 100);
	//static_assert(offsetof(PopAnim, mAnimRunning) == 157);
}