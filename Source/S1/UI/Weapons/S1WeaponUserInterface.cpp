
// Copyright (c) S1

#include "S1WeaponUserInterface.h"
#include "Weapons/S1WeaponInstance.h"
#include "Equipment/S1EquipmentManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1WeaponUserInterface)

US1WeaponUserInterface::US1WeaponUserInterface(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1WeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Pawn을 가져오고
    if (APawn* Pawn = GetOwningPlayerPawn())
    {
        // EquipmentManagerComponent를 활용해, WeaponInstance를 가져오고
        if (US1EquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<US1EquipmentManagerComponent>())
        {
            if (US1WeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<US1WeaponInstance>())
            {
                if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
                {
                    // 새로 업데이트 해주고, OnWeaponChanged를 호출
                    US1WeaponInstance* OldWeapon = CurrentInstance;
                    CurrentInstance = NewInstance;
                    OnWeaponChanged(OldWeapon, CurrentInstance);
                }
            }
        }
    }
}


