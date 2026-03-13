#include "ZombieFairyTaleWitchFrog.h"
#include "Plant.h"
#include "AddPlantType.h"

DEFINE_STATIC_RT_CLASS_MEMBERS(ZombieFairyTaleWitchFrog)

void overrideFrogActionFrame(ZombieFairyTaleWitchFrog* self, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim) {
    if (*actionName == "respawn") {
        float posX = self->m_position.x;
        float posY = self->m_position.y;
        int row = (((int)posY - 540) / 76) + 4;
        int column = (int)((posX - 200.0f) / 64.0f);
        if (column >= 0 && column <= 8 && row >= 0 && row <= 4) {
            AddPlant(self->m_transformedPlant, column, row);
        }
    }
}
void ZombieFairyTaleWitchFrog::ModInit() {
    LOGI("ZombieFairyTaleWitchFrog init");

    vftable = CopyVFTable(getActualOffset(0x241D430), 204);

    PatchVFTable(vftable, (void*)ZombieFairyTaleWitchFrog::StaticGetType, 0);

    PatchVFTable(vftable, (void*)overrideFrogActionFrame, 170);

    ZombieFairyTaleWitchFrog::StaticGetType();

    LOGI("ZombieFairyTaleWitchFrog finish init");
}
