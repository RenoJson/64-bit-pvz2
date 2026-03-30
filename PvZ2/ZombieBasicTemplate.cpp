#include "ZombieBasic.h"
#include "ZombieBasicProps.h"
#include "ZombieAnimRig_Gargantuar.h"
#include "ZombieAnimRig_Basic.h"

void* ZombieBasicTemplate::vftable = __null;
Sexy::RtClass* ZombieBasicTemplate::s_rtClass = __null;;

void overrideBOnSpawn(ZombieBasicTemplate* zombie) {
	auto props = reinterpret_cast<ZombieBasicProps*>(zombie->m_propertySheet.Get());
	auto rigProps = reinterpret_cast<ZombieAnimRigTemplateConfig*>(props->CustomAnimRigPropertySheet.Get());
	auto rig = reinterpret_cast<ZombieAnimRig_BasicTemplate*>(zombie->m_animRig.Get());
	rig->m_LowerArmLayers = rigProps->LowerArmLayers;
	rig->m_HeadLayers = rigProps->HeadLayers;
	rig->m_ParticleArmSpriteName = rigProps->ParticleArmSpriteName;
	rig->m_ParticleHeadSpriteName = rigProps->ParticleHeadSpriteName;
	rig->m_WalkAnimName = rigProps->WalkAnimName;
	rig->m_EatAnimName = rigProps->EatAnimName;
	rig->m_DieAnimName = rigProps->DieAnimName;
	typedef void (*zombieFun49)(ZombieBasicTemplate*);
	((zombieFun49)getActualOffset(0xC3D1F0))(zombie);
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

	PatchVFTable(vftable, (void*)overrideBOnSpawn, 49);

	PatchVFTable(vftable, (void*)hkAnimShock, 189);

	PatchVFTable(vftable, (void*)hkAnimAsh, 190);

	ZombieBasicTemplate::StaticGetType();

	LOGI("ZombieBasicTemplate finish init");
}