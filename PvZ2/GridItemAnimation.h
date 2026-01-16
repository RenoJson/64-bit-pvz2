#pragma once
#include "GridItem.h"

class PopAnimRig;

#define GRIDITEM_ANIM_CTOR 0x76CE8C
#define GRIDITEM_ANIM_DTOR 0x76D350
#define GRIDITEM_ANIM_GETTYPE 0x76CE24
#define GRIDITEM_ANIM_VFTABLE 0x2396530
#define GRIDITEM_ANIM_VFUNCS 68

class GridItemAnimation : public GridItem
{
public:
	bool m_hasLinkedAnimRig;
	char m_pad6[4];
	Sexy::RtWeakPtr<PopAnimRig> m_animRig;
};

static_assert(sizeof(GridItemAnimation) == 320);
static_assert(offsetof(GridItemAnimation, m_hasLinkedAnimRig) == 304);
static_assert(offsetof(GridItemAnimation, m_animRig) == 312);

#define GRIDITEM_ANIMPROPS_CTOR 0x76E6E4
#define GRIDITEM_ANIMPROPS_DTOR 0x6BD174
#define GRIDITEM_ANIMPROPS_GETTYPE 0x76C6AC
#define GRIDITEM_ANIMPROPS_VFTABLE 0x2396790
#define GRIDITEM_ANIMPROPS_VFUNCS 14

class GridItemAnimationProps : public GridItemPropertySheet
{
public:
	SexyString PopAnim;
	SexyString PopAnimRigClass;
	Sexy::SexyVector2 PopAnimRenderOffset;
	Sexy::SexyVector2 PopAnimRenderScale;
	SexyString LinkedPopAnim;
	Sexy::SexyVector3 LinkedOffset;
	float LinkedLayerOffset;
};

static_assert(sizeof(GridItemAnimationProps) == 256);
static_assert(offsetof(GridItemAnimationProps, PopAnim) == 152);
static_assert(offsetof(GridItemAnimationProps, LinkedOffset) == 240);