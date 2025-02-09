
// Copyright (c) S1

#pragma once

#include "Components/ControllerComponent.h"
#include "S1QuickBarComponent.generated.h"

/** foward declarations */
class US1InventoryItemInstance;
class US1EquipmentInstance;
class US1EquipmentManagerComponent;

/*
 * HUD의 QuickBar
 *  - MMORPG에서 ShorCut HUD
 *  - HUD(Slate)와 Inventory/Equipment(Gameplay)의 Bridge 역할을 수행하는 Component
 *  - 해당 Component는 HUD 및 Slate Widget에서 사용함
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class S1_API US1QuickBarComponent : public UControllerComponent
{
    GENERATED_BODY()

public:
    US1QuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    /*
     * UControllerComponent`s Interface
     */
    virtual void BeginPlay() final;

    void UnequipItemSlot();
    void EquipItemSlot();

    US1EquipmentManagerComponent* FindEquipmentMAnager();

    UFUNCTION(BlueprintCallable)
    void AddItemToSlot(int32 SlotIndex, US1InventoryItemInstance* Item);

    UFUNCTION(BlueprintCallable)
    void SetSlotActivate(int32 SlotIndex);

    TSubclassOf<US1CameraMode> GetZoomMode();

    /** HUD QuickBar Slot 개수 */
    UPROPERTY()
    int32 NumSlots = 3;

    /** HUD QuickBar Slot 리스트 */
    UPROPERTY()
    TArray<TObjectPtr<US1InventoryItemInstance>> Slots;

    /** 현재 활성화된 Slot Index */
    UPROPERTY()
    int32 ActiveSlotIndex = -1;
    

    /** 현재 장착한 장비 정보 */
    UPROPERTY()
    TObjectPtr<US1EquipmentInstance> EquippedItem;

}; 
