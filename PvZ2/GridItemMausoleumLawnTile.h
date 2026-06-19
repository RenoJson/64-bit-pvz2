#pragma once
#include "GridItem.h"
#include <Sexy/Image.h>

class ResourceInfoType__ImageRes {
public:
	RT_CLASS_GET_CLASS_WRAPPER(0x1640D3C);
};

class GridItemMausoleumLawnTile : public GridItem {
public:
	Sexy::RtWeakPtr<Image> m_imageRes;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemMausoleumLawnTile, 0x805C04);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItem);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(GridItemMausoleumLawnTile, m_imageRes, Image)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemMausoleumLawnTile);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemMausoleumLawnTile, 0x8051EC);
};

class GridItemMausoleumLawnTileProps : public GridItemPropertySheet {
public:
	SexyString Image;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemMausoleumLawnTileProps, 0x6BD264);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemPropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(GridItemMausoleumLawnTileProps, Image);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemMausoleumLawnTileProps);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemMausoleumLawnTileProps, 0x1267EB0);
};

class GridItemMausoleumLawnPath : public GridItem {
public:
	Sexy::RtWeakPtr<Image> m_imageRes;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemMausoleumLawnPath, 0x805C04);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItem);
	RT_CLASS_REGISTER_CLASS_RTWEAKPTR_PROPERTY(GridItemMausoleumLawnPath, m_imageRes, Image)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemMausoleumLawnPath);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemMausoleumLawnPath, 0x8051EC);
};

class GridItemMausoleumLawnPathProps : public GridItemPropertySheet {
public:
	std::vector<int> DirectionType;
	SexyString Image;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(GridItemMausoleumLawnTileProps, 0x6BD264);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(GridItemPropertySheet);
	RT_CLASS_REGISTER_STRING_PROPERTY(GridItemMausoleumLawnPathProps, Image);
	RT_CLASS_REGISTER_STANDARD_VECTOR_PROPERTY(GridItemMausoleumLawnPathProps, DirectionType, int)
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(GridItemMausoleumLawnTileProps);
	RT_CLASS_GET_CLASS_FUNCTION(GridItemMausoleumLawnTileProps, 0x1267EB0);
};