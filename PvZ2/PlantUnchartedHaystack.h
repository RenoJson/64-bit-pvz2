
#include "PvZ2/PlantFramework.h"
#include "PvZ2/GridItem.h"
#include <Sexy/RtReflectionDelegateBase.h>
#include <Reflection/ReflectionBuilder.h>
#include "GridItemHaystack.h"


class PlantUnchartedHaystack : public PlantFramework
{
public:
	GridItemHaystack* m_gridItem;
	static void* vftable;
	static Sexy::RtClass* s_rtClass;
	static void modInit();;

	RT_CLASS_CONSTRUCT_FUNCTION_BEGIN(PlantUnchartedHaystack, 0x1639F68);
	RT_CLASS_CONSTRUCT_FUNCTION_END();

	RT_CLASS_BUILD_SYMBOLS_BEGIN(PlantFramework);
	RT_CLASS_BUILD_SYMBOLS_END();

	RT_CLASS_REGISTER_CLASS_FUNCTION(PlantUnchartedHaystack);
	RT_CLASS_GET_CLASS_FUNCTION(PlantUnchartedHaystack, 0x7C6F38);
};