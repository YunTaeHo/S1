
// Copyright (c) S1

#include "S1ReticleWidgetBase.h"
#include "Weapons/S1WeaponInstance.h"
#include "Inventory/S1InventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ReticleWidgetBase)

US1ReticleWidgetBase::US1ReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1ReticleWidgetBase::InitializeFromWeapon(US1WeaponInstance* InWeapon)
{
    WeaponInstance = InWeapon;
    InventoryInstance = nullptr;

    if (WeaponInstance)
    {
        InventoryInstance = Cast<US1InventoryItemInstance>(WeaponInstance->GetInstigator());
    }
    OnWeaponInitialized();
}


