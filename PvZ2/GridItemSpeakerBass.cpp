
#include "AddProjectileType.h"
#include "GridItemSpeakerBass.h"
#include "ZombieHelper.h"
#include "ZombieStateHelper.h"

void* GridItemSpeakerBass::vftable = nullptr;
Sexy::RtClass* GridItemSpeakerBass::s_rtClass = nullptr;;

void* GridItemSpeakerBassProps::vftable = nullptr;
Sexy::RtClass* GridItemSpeakerBassProps::s_rtClass = nullptr;;

void SpeakerBassOnPopAnimCommand(GridItemSpeakerBass* speaker, const SexyString& animName, const SexyString& actionName) {
	if (actionName == "use_action") {
        auto props = reinterpret_cast<GridItemSpeakerBassProps*>(speaker->m_propertySheet.Get());
        Projectile* shockwave = AddProjectile(&props->ShockWaveProjectile,
            speaker,
            speaker->m_position.x + props->ShockWaveSpawnOffset.x,
            speaker->m_position.y + props->ShockWaveSpawnOffset.y,
            0);
        shockwave->m_teamFlags = speaker->m_teamFlags;
	}
}

void GridItemSpeakerBass::SpeakerSetState(GridItemSpeakerBass* speaker, int state)
{
    speaker->m_speakerState = state;
    switch (state) {
    case 5:
        auto rig = reinterpret_cast<PopAnimRig*>(speaker->m_animRig.Get());
        auto dlgtEvent = RegisterDelegateEvent(speaker, "onAnimEnded");
        PlayAndStopAnim(rig, "flash", 0, dlgtEvent);
        break;
    }
    CallFunc<void>(0xD9ACE8, speaker, state);
}
void GridItemSpeakerBass::modInit() {
    vftable = CopyVFTable(getActualOffset(0x2431560), 73);
    PatchVFTable(vftable, (void*)GridItemSpeakerBass::StaticGetType, 0);
    PatchVFTable(vftable, (void*)SpeakerBassOnPopAnimCommand, 71);
    PatchVFTable(vftable, (void*)SpeakerSetState, 72);
    GridItemSpeakerBass::StaticGetType();
}

void GridItemSpeakerBassProps::modInit() {
    vftable = CopyVFTable(getActualOffset(0x24317E8), 14);
    PatchVFTable(vftable, (void*)GridItemSpeakerBassProps::StaticGetType, 0);
    GridItemSpeakerBassProps::StaticGetType();
}
