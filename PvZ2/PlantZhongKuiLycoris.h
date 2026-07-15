
#include "PvZ2/PlantFramework.h"
#include "PvZ2/Plant.h"
#include <Sexy/RtReflectionDelegateBase.h>
#include <Reflection/ReflectionBuilder.h>


class PlantZhongKuiLycoris : public PlantFramework
{
public:
	int m_actionIndex;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(PlantZhongKuiLycoris, 0x1639F68);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantFramework);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(PlantZhongKuiLycoris);
	RT_CLASS_GET_CLASS_FUNCTION(PlantZhongKuiLycoris, 0x7C6F38);
};

class PlantZhongKuiLycoris : public PlantFramework
{
public:
	int m_actionIndex;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(PlantZhongKuiLycoris, 0x1639F68);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantFramework);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(PlantZhongKuiLycoris);
	RT_CLASS_GET_CLASS_FUNCTION(PlantZhongKuiLycoris, 0x7C6F38);
};

class ProjectileActionProps : public PropertySheetBase {
public:
	int Index;
	float Weight;
	SexyString ProjectileLayerToShow;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ProjectileActionProps, 0x8AE3A8);
	RT_CLASS_CONSTRUCT_FUNCTION_END();
	RT_CLASS_BUILD_SYMBOLS_BEGIN(PropertySheetBase);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ProjectileActionProps, Index);
	RT_CLASS_REGISTER_STANDARD_PROPERTY(ProjectileActionProps, Weight);
	RT_CLASS_REGISTER_STRING_PROPERTY(ProjectileActionProps, ProjectileLayerToShow);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ProjectileActionProps);
	RT_CLASS_GET_CLASS_FUNCTION(ProjectileActionProps, 0xD1FB60);
};
class ZhongKuiLycorisProps : public PlantPropertySheet {
public:
	std::vector<ProjectileActionProps> ProjectileActionWeight;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(ZhongKuiLycorisProps, 0x611410);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantPropertySheet);
	RT_CLASS_REGISTER_CLASS_VECTOR_PROPERTY(ZhongKuiLycorisProps, ProjectileActionWeight, ProjectileActionProps)
		RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(ZhongKuiLycorisProps);
	RT_CLASS_GET_CLASS_FUNCTION(ZhongKuiLycorisProps, 0x61E4B0);

};