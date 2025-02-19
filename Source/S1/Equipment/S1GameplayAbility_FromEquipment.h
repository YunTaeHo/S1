
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/S1GameplayAbility.h"
#include "S1GameplayAbility_FromEquipment.generated.h"

/** foward declarations */
class US1EquipmentInstance;
class US1InventoryItemInstance;

UCLASS()
class S1_API US1GameplayAbility_FromEquipment : public US1GameplayAbility
{
    GENERATED_BODY()

public:
    US1GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    US1EquipmentInstance* GetAssociatedEquipment() const;
    UFUNCTION(BlueprintCallable)
    US1InventoryItemInstance* GetAssociatedItem() const;
}; 
