#include "PlantChomper.h"
#include "Creature.h"


void* PlantDinoGrass::vftable = __null;
Sexy::RtClass* PlantDinoGrass::s_rtClass = __null;;

typedef void (*creatureEnterState)(Dinosaur*, int, int);
typedef void (*chomperDoSpecial)(PlantChomper*);
typedef void (*chomperOnAnimCommand)(PlantChomper*, SexyString*);
typedef void (*GetEntitiesInRectFunc)(std::vector<BoardEntity*>*, int, Rect*);


void DinoGrassOnAnimCommand(PlantDinoGrass* thisPtr, SexyString* command) {
	((chomperOnAnimCommand)getActualOffset(0xEDA2BC))(thisPtr, command);
	if (*command == "roar_start") {
        int chomperRow = thisPtr->m_plant->m_row;
        int chomperCol = thisPtr->m_plant->m_column;

        Rect scanRect;
        scanRect.mX = chomperCol;
        scanRect.mY = chomperRow - 2;
        scanRect.mWidth = 5;
        scanRect.mHeight = 5;

        if (scanRect.mY < 0) scanRect.mY = 0;
        else if (scanRect.mY + scanRect.mHeight > 5) scanRect.mY = 5 - scanRect.mHeight;

        if (scanRect.mX < 0)
        {
            scanRect.mX = 0;
        }
        else if (scanRect.mX + scanRect.mWidth > 11)
        {
            scanRect.mX = 11 - scanRect.mWidth;
        }

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
                setZCondition(zombie, zombie_condition_stun, 0, 5.0f, 0.0f);
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