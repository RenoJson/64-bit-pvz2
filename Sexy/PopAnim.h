#pragma once
#include "BaseResource.h"
#include "PopAnimListener.h"

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
		int mId;
		PopAnimListener* mPopAnimListener;
		float mDrawScale;
		float mImgScale;
		int mAnimRate;
		Rect mAnimRect;
		SexyString mActiveAnimLabel;
		void* mMainSpriteInst;
		void* mMainAnimDef;
		float mBlendTicksTotal;
		float mBlendTicksCur;
		float mBlendDelay;
		SexyTransform2D mTransform;
		Color mColor;
		bool mAdditive;
		bool mNeedsUpdateTransforms;
		bool mAnimRunning;
		bool mPaused;
		bool mColorizeType;
	};

	//static_assert(sizeof(PopAnim) == 160);
	//static_assert(offsetof(PopAnim, mDrawScale) == 40);
	//static_assert(offsetof(PopAnim, mAnimRect) == 52);
	//static_assert(offsetof(PopAnim, mTransform) == 100);
	//static_assert(offsetof(PopAnim, mAnimRunning) == 157);
}