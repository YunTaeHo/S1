
// Copyright (c) S1

#pragma once

#include "S1InventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "S1InventoryFragment_EquippableItem.generated.h"

/** forward declaractions */
class US1EquipmentDefinition;

UCLASS()
class S1_API US1InventoryFragment_EquippableItem : public US1InventoryItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "S1")
    TSubclassOf<US1EquipmentDefinition> EquipmentDefinition;

}; 
