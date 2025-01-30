
// Copyright (c) S1

#pragma once

#include "Components/ControllerComponent.h"
#include "S1InventoryManagerComponent.generated.h"

/** forwad declarations */
class US1InventoryItemInstance;
class US1InventoryItemDefinition;

USTRUCT(BlueprintType)
struct FS1InventoryEntry
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<US1InventoryItemInstance> Instance = nullptr;
};

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FS1InventoryList
{
    GENERATED_BODY()

    FS1InventoryList(UActorComponent* InOwnerComponent = nullptr)
        : OwnerComponent(InOwnerComponent)
    {}

    US1InventoryItemInstance* AddEntry(TSubclassOf<US1InventoryItemDefinition> ItemDef);

    UPROPERTY()
    TArray<FS1InventoryEntry> Entries;

    UPROPERTY()
    TObjectPtr<UActorComponent> OwnerComponent = nullptr;
};

/*
 *  PlayerController의 Component로서 Inventory를 관리
 */
UCLASS()
class S1_API US1InventoryManagerComponent : public UControllerComponent
{
    GENERATED_BODY()

public:
    US1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** InventoryItemDefinition을 통해, ItemInstance를 반환 */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    US1InventoryItemInstance* AddItemDefinition(TSubclassOf<US1InventoryItemDefinition> ItemDef);

    UPROPERTY()
    FS1InventoryList InventoryList;
}; 
