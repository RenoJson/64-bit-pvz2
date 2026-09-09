#include "ZombieBasic.h"
#include "ZombieBasicProps.h"
#include "ZombieAnimRig_Gargantuar.h"
#include "ZombieAnimRig_Basic.h"

void* ZombieBasicTemplate::vftable = __null;
Sexy::RtClass* ZombieBasicTemplate::s_rtClass = __null;;

Zombie* BInitialize(ZombieBasicTemplate* zombie) {
	auto props = reinterpret_cast<ZombieBasicProps*>(zombie->m_propertySheet.Get());
	auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(props->AnimRigProps.Get());
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	typedef Zombie* (*update)(ZombieBasicTemplate*);
	return ((update)getActualOffset(0xB53770))(zombie);

}
SexyString hkAnimShock(ZombieBasicTemplate* zombie) {
	auto* getProps = reinterpret_cast<ZombieBasicProps*>(zombie->m_propertySheet.Get());
	return getProps->ShockAnimName;
}

SexyString hkAnimAsh(ZombieBasicTemplate* zombie) {
	auto* getProps = reinterpret_cast<ZombieBasicProps*>(zombie->m_propertySheet.Get());
	return getProps->AshAnimName;
}
void ZombieBasicTemplate::modInit() {
	LOGI("ZombieBasicTemplate init");

	vftable = CopyVFTable(getActualOffset(0x23DEFB8), 206);

	PatchVFTable(vftable, (void*)ZombieBasicTemplate::StaticGetType, 0);

	PatchVFTable(vftable, (void*)BInitialize, 169);

	PatchVFTable(vftable, (void*)hkAnimShock, 189);

	PatchVFTable(vftable, (void*)hkAnimAsh, 190);

	ZombieBasicTemplate::StaticGetType();

	LOGI("ZombieBasicTemplate finish init");
}