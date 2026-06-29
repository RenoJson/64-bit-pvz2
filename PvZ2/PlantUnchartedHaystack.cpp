#include "PlantUnchartedHaystack.h"
#include "Plant.h"
#include "AddGridItemType.h"


void* PlantUnchartedHaystack::vftable = __null;
Sexy::RtClass* PlantUnchartedHaystack::s_rtClass = __null;


uint HaystackRenderOrder() {
	return 250000;
}
bool HaystackCanUsePlantfood() {
	return false;
}
bool HaystackCanBeShoveled() {
	return false;
}
bool HaystackHasShadow() {
	return true;
}

void PlantUnchartedHaystack::modInit() {
	LOGI("PlantUnchartedHaystack init");
	vftable = CopyVFTable(getActualOffset(0x239BDA0), 121);
	PatchVFTable(vftable, (void*)PlantUnchartedHaystack::StaticGetType, 0);
	PatchVFTable(vftable, (void*)HaystackRenderOrder, 12);
	PatchVFTable(vftable, (void*)HaystackCanUsePlantfood, 40);
	PatchVFTable(vftable, (void*)HaystackHasShadow, 60);
	PlantUnchartedHaystack::StaticGetType();
	LOGI("PlantUnchartedHaystack finish init");
}