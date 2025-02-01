
// Copyright (c) S1

#pragma once

#include "GameplayTagContainer.h"
#include "S1InventoryItemDefinition.h"
#include "S1InventoryFragment_SetStats.generated.h"

/** foward declarations */
class US1InventoryItemInstance;

UCLASS()
class S1_API US1InventoryFragment_SetStats : public US1InventoryItemFragment
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(US1InventoryItemInstance* Instance) const override;

    /** InitialItemStats는 TagStackContainer를 위해 Fragment에서 추가해준다 */
    UPROPERTY(EditDefaultsOnly, Category = "Equipment")
    TMap<FGameplayTag, int32> InitialItemStats;

}; 
