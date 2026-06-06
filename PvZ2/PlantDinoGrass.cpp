#include "PlantChomper.h"
#include "ChomperProps.h"
#include "Creature.h"


void* PlantDinoGrass::vftable = __null;
Sexy::RtClass* PlantDinoGrass::s_rtClass = __null;;

void* DinoGrassProps::vftable = __null;
Sexy::RtClass* DinoGrassProps::s_rtClass = __null;;

typedef void (*creatureEnterState)(Dinosaur*, int, int);
typedef void (*chomperDoSpecial)(PlantChomper*);
typedef void (*chomperOnAnimCommand)(PlantChomper*, SexyString*);
typedef void (*GetEntitiesInRectFunc)(std::vector<BoardEntity*>*, int, Rect*);


void DinoGrassOnAnimCommand(PlantDinoGrass* thisPtr, SexyString* command) {
	((chomperOnAnimCommand)getActualOffset(0xEDA2BC))(thisPtr, command);
    if (*command == "roar_start") {
        auto* props = static_cast<DinoGrassProps*>(thisPtr->m_plant->m_propertySheet.Get());
        if (props == nullptr || props->Actions.size() <= 1) return;

        int chomperRow = thisPtr->m_plant->m_row;
        int chomperCol = thisPtr->m_plant->m_column;

        Rect scanRect;
        scanRect.mWidth = props->Actions[1].RectTriggerRange.mWidth;
        scanRect.mHeight = props->Actions[1].RectTriggerRange.mHeight;
        scanRect.mX = chomperCol + props->Actions[1].RectTriggerRange.mX;
        scanRect.mY = chomperRow + props->Actions[1].RectTriggerRange.mY;

        if (scanRect.mY + scanRect.mHeight > 5) scanRect.mY = 5 - scanRect.mHeight;
        if (scanRect.mY < 0) scanRect.mY = 0;

        if (scanRect.mX + scanRect.mWidth > 11) scanRect.mX = 11 - scanRect.mWidth;
        if (scanRect.mX < 0) scanRect.mX = 0;

        std::vector<BoardEntity*> entityList;
        GetEntitiesInRectFunc getEntitiesRect = (GetEntitiesInRectFunc)getActualOffset(0x86F180);
        getEntitiesRect(&entityList, 63, &scanRect);

        for (BoardEntity* entity : entityList)
        {
            if (entity != nullptr && entity->IsType(Zombie::StaticGetType()))
            {
                Zombie* zombie = static_cast<Zombie*>(entity);
                typedef void (*setConditionZ)(Zombie*, int, int, float, float);
                setConditionZ setZCondition = (setConditionZ)getActualOffset(0xC40CC0);

                int dist = static_cast<int>((zombie->m_position.x - 200.0f) / 64.0f) - chomperCol;
                if (dist < 0) dist = 0;

                int maxIndex = props->StunDurationRange.size() - 1;
                if (maxIndex < 0) maxIndex = 0;
                if (dist > maxIndex) dist = maxIndex;

                float stunTime = props->Actions[1].StunDuration;
                if (dist < props->StunDurationRange.size()) {
                    stunTime = props->StunDurationRange[dist];
                }

                setZCondition(zombie, zombie_condition_stun, 0, stunTime, 0.0f);
            }
            else if (entity != nullptr && entity->IsType(DinosaurRaptor::StaticGetType()))
            {
                DinosaurRaptor* dino = static_cast<DinosaurRaptor*>(entity);
                ((creatureEnterState)getActualOffset(0xD435FC))(dino, 5, 0);
            }
            else if (entity->IsType(DinosaurStegosaurus::StaticGetType()))
            {
                DinosaurStegosaurus* dino = static_cast<DinosaurStegosaurus*>(entity);

                if (dino->m_zombiesOnTail.empty() && dino->m_zombiesInAttackRange.empty())
                {
                    ((creatureEnterState)getActualOffset(0xD435FC))(reinterpret_cast<Dinosaur*>(dino), 11, 0);
                }
            }
            else if (entity != nullptr && entity->IsType(DinosaurPterodactyl::StaticGetType()))
            {
                DinosaurPterodactyl* dino = static_cast<DinosaurPterodactyl*>(entity);
                dino->m_timeout = 0.0f;
            }
            else if (entity != nullptr && entity->IsType(DinosaurTyrannosaurus::StaticGetType()))
            {
                DinosaurTyrannosaurus* dino = static_cast<DinosaurTyrannosaurus*>(entity);
                ((creatureEnterState)getActualOffset(0xD435FC))(dino, 13, 0);
            }
            else if (entity != nullptr && entity->IsType(DinosaurAnkylosaurus::StaticGetType()))
            {
                DinosaurAnkylosaurus* dino = static_cast<DinosaurAnkylosaurus*>(entity);
                ((creatureEnterState)getActualOffset(0xD435FC))(dino, 9, 0);
            }
        }
    }
}
void PlantDinoGrass::modInit() {
	LOGI("PlantDinoGrass init");
	vftable = CopyVFTable(getActualOffset(0x24631A8), 121);
	PatchVFTable(vftable, (void*)PlantDinoGrass::StaticGetType, 0);
    PatchVFTable(vftable, (void*)DinoGrassOnAnimCommand, 86);
	PlantDinoGrass::StaticGetType();
	LOGI("PlantDinoGrass finish init");
}
void DinoGrassProps::modInit() {
    LOGI("DinoGrassProps init");
    vftable = CopyVFTable(getActualOffset(0x237E8D8), 14);
    PatchVFTable(vftable, (void*)DinoGrassProps::StaticGetType, 0);
    DinoGrassProps::StaticGetType();
    LOGI("DinoGrassProps finish init");
}