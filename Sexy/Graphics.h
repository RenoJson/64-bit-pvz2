#pragma once
#include "Image.h"
namespace Sexy
{
	class Image;

	class GraphicsState
	{
	public:
		void** vftable;
		Image* mDestImage;
		float mTransX;
		float mTransY;
		float mScaleX;
		float mScaleY;
		float mScaleOrigX;
		float mScaleOrigY;
		Rect mClipRect;
		Color mColor;
		void* mFont; // Sexy::Font*
		int mDrawMode;
		bool mColorizeImages;
		bool mFastStretch;
		bool mWriteColoredString;
		bool mLinearBlend;
		bool mIs3D;
	};


	typedef std::list<GraphicsState> GraphicsStateList;

	class Graphics : public GraphicsState
	{
	public:
		void* mRenderDevice; // AndroidRenderDevice*
		int mRenderContext;
		int mGraphics3D;
		void* mPFActiveEdgeList; // Edge*
		int	mPFNumActiveEdges;
		int	mPFNumVertices;
		GraphicsStateList mStateStack;

	};

	static_assert(sizeof(Graphics) == 152);
}