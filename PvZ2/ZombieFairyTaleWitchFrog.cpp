#include "ZombieFairyTaleWitchFrog.h"
#include "AddPlantType.h"


DEFINE_STATIC_RT_CLASS_MEMBERS(ZombieFairyTaleWitchFrog)

void overrideFrogActionFrame(ZombieFairyTaleWitchFrog* self, int64_t unk1, SexyString* actionName, int64_t unk2, SexyString* currentAnim) {
    float posX = self->m_position.x;
    float posY = self->m_position.y;
    int row = (((int)posY - 540) / 76) + 4;
    int column = (int)((posX - 200.0f) / 64.0f);
    if (*actionName == "respawn" && column >= 0 && column <= 8 && row >= 0 && row <= 4) {
        std::vector<SexyString> typenameList = {
            "puffshroom",
            "sunshroom",
            "scaredyshroom",
            "magnetshroom",
            "fumeshroom"
        };
        auto getRandString = [&](const std::vector<SexyString>&typenameList) {
            int randomIndex = rand() % typenameList.size();
            return typenameList[randomIndex];
        };
        SexyString plantTypeName = getRandString(typenameList);
        AddPlant(plantTypeName, column, row);
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
