
// Copyright (c) S1

#pragma once

#include "Components/ControllerComponent.h"
#include "S1QuickBarComponent.generated.h"

/** foward declarations */
class US1InventoryItemInstance;
class US1EquipmentInstance;
class US1EquipmentManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotChanged, int32, SlotIndex, bool, IsEmpty);

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

    UFUNCTION(BlueprintCallable, BlueprintPure = false)
    TArray<US1InventoryItemInstance*> GetSlots() const
    {
        return Slots;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure = false)
    int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

    TSubclassOf<US1CameraMode> GetZoomMode();

    UFUNCTION(BlueprintCallable, BlueprintPure = false)
    US1InventoryItemInstance* GetActiveSlotItem() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false)
    int32 GetNextFreeItemSlot() const;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    US1InventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

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

    /** Slot이 바뀔때마다 호출, Index에 맞는 것만 바꿔주자 */
    UPROPERTY(BlueprintAssignable)
    FOnSlotChanged OnSlotChanged;

}; 
