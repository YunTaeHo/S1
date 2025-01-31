
// Copyright (c) S1

#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"
#include "S1EquipmentDefinition.h"
#include "S1EquipmentInstance.generated.h"


UCLASS(BlueprintType, Blueprintable)
class S1_API US1EquipmentInstance : public UObject
{
    GENERATED_BODY()

public:
    US1EquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintPure, Category = "Equipment")
    APawn* GetPawn() const;

    UFUNCTION(BlueprintPure, Category = "Equipment")
    UObject* GetInstigator() const { return Instigator; }
    
    /*
     * DeterminesOutputType은 C++ 정의에는 APawn*을 반환하지만, 
     * BP에서는 PawnType에 따라 OutputType이 결정되도록 Redirect 시켜준다
     */
    UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = "PawnType"))
    APawn* GetTypePawn(TSubclassOf<APawn> PawnType) const;

    void SpawnEquipmentActors(const TArray<FS1EquipmentActorToSpawn>& ActorsToSpawn);
    void DestroyEquipmentActors();

    /*
     * Blueprint 정의를 위한 Equip/Unequip 함수
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "On Equipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "On Unequipped"))
    void K2_OnUnequipped();

    UFUNCTION(BlueprintPure, Category = "Equipment")
    TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

    /*
     * Interfaces
     */
    virtual void OnEquipped();
    virtual void OnUnequipped();

    /** 어떤 InventoryItemInstance에 의해 활성화되었는지 (QuickBarComponent) */
    UPROPERTY()
    TObjectPtr<UObject> Instigator;

    /** S1EquipmentDefinition에 맞게 Spawn된 Actor Instances */
    UPROPERTY()
    TArray<TObjectPtr<AActor>> SpawnedActors;
}; 
