
// Copyright (c) S1

#include "S1QuickBarComponent.h"
#include "S1EquipmentManagerComponent.h"
#include "S1EquipmentInstance.h"
#include "S1EquipmentDefinition.h"
#include "Inventory/S1InventoryItemInstance.h"
#include "Inventory/S1InventoryFragment_EquippableItem.h"
#include "Camera/S1CameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1QuickBarComponent)

US1QuickBarComponent::US1QuickBarComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1QuickBarComponent::BeginPlay()
{
    // NumSlots에 따라 미리 Slots을 할당한다
    if (Slots.Num() < NumSlots)
    {
        Slots.AddDefaulted(NumSlots - Slots.Num());
    }

    Super::BeginPlay();
}


void US1QuickBarComponent::EquipItemSlot()
{
    check(Slots.IsValidIndex(ActiveSlotIndex));
    check(EquippedItem == nullptr);

    // 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryItemInstance를 찾는다
    if (US1InventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
    {
        // Slot Item을 통해 US1InventoryFragment_EquippableItem를 찾는다 
        //  - 찾는 것이 실패했다면 Equip할 수 없다는 의미
        if (const US1InventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<US1InventoryFragment_EquippableItem>())
        {
            // EquippableItem에서 EqupmentDefinition을 찾는다
            // - EquopmentDefinition이 있어야, 장착할 수 있다
            TSubclassOf<US1EquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
            if (EquipDef)
            {
                // EquipmentManager를 가져오고
                if (US1EquipmentManagerComponent* EquipmentManager = FindEquipmentMAnager())
                {
                    // EquipmentManager를 통해 Pawn의 장비를 장착
                    EquippedItem = EquipmentManager->EquipItem(EquipDef);
                    if (EquippedItem)
                    {
                        // Controller에도 EquipItem의 상태를 업데이트
                        EquippedItem->Instigator = SlotItem;
                    }
                }
            }
        }
    }
}

void US1QuickBarComponent::UnequipItemSlot()
{
    // EquipmentManager를 가져오고
    if (US1EquipmentManagerComponent* EquipmentManager = FindEquipmentMAnager())
    {
        // 현재 장착된 Item이 있다면
        if (EquippedItem)
        {
            // EquipmentManager를 통해 Pawn의 장비를 해제
            EquipmentManager->UnequipItem(EquippedItem);

            // Controller에도 EquipItem의 상태를 없는 것으로 업데이트
            EquippedItem = nullptr;
        }
    }
}

US1EquipmentManagerComponent* US1QuickBarComponent::FindEquipmentMAnager()
{
    // 둘 다 ControllerComponent이기 때문에 Owner를 바탕으로 가져오자
    if (AController* OwnerController = Cast<AController>(GetOwner()))
    {
        if (APawn* Pawn = OwnerController->GetPawn())
        {
            return Pawn->FindComponentByClass<US1EquipmentManagerComponent>();
        }
    }

    return nullptr;
}

void US1QuickBarComponent::AddItemToSlot(int32 SlotIndex, US1InventoryItemInstance* Item)
{
    // Pre-size한 Slots에 Item을 넣어준다
    if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
    {
        if (Slots[SlotIndex] == nullptr)
        {
            Slots[SlotIndex] = Item;
            OnSlotChanged.Broadcast(SlotIndex, false);
            //FLyraQuickBarSlotsChangedMessage Message;
            //Message.Owner = GetOwner();
            //Message.Slots = Slots;

            //UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
            //MessageSystem.BroadcastMessage(TAG_Lyra_QuickBar_Message_SlotsChanged, Message);
        }
    }
}

void US1QuickBarComponent::SetSlotActivate(int32 SlotIndex)
{
    if (Slots.IsValidIndex(SlotIndex) && (ActiveSlotIndex != SlotIndex))
    {
        // UnequipItem/EquipItem에서 NewIndex를 통해 할당된 Item을 장착 및 업데이트 진행
        UnequipItemSlot();
        ActiveSlotIndex = SlotIndex;
        EquipItemSlot();
        OnSlotChanged.Broadcast(SlotIndex, false);
        //FLyraQuickBarActiveIndexChangedMessage Message;
        //Message.Owner = GetOwner();
        //Message.ActiveIndex = ActiveSlotIndex;

        //UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
        //MessageSystem.BroadcastMessage(TAG_Lyra_QuickBar_Message_ActiveIndexChanged, Message);
    }
}

TSubclassOf<US1CameraMode> US1QuickBarComponent::GetZoomMode()
{
    return EquippedItem->ZoomMode;
}



US1InventoryItemInstance* US1QuickBarComponent::GetActiveSlotItem() const
{
    return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 US1QuickBarComponent::GetNextFreeItemSlot() const
{
    int32 SlotIndex = 0;
    for (const TObjectPtr<US1InventoryItemInstance>& ItemPtr : Slots)
    {
        if (ItemPtr == nullptr)
        {
            return SlotIndex;
        }
        ++SlotIndex;
    }

    return INDEX_NONE;
}


US1InventoryItemInstance* US1QuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
    US1InventoryItemInstance* Result = nullptr;

    if (ActiveSlotIndex == SlotIndex)
    {
        UnequipItemSlot();
        ActiveSlotIndex = -1;
    }

    if (Slots.IsValidIndex(SlotIndex))
    {
        Result = Slots[SlotIndex];

        if (Result != nullptr)
        {
            Slots[SlotIndex] = nullptr;
            OnSlotChanged.Broadcast(SlotIndex, true);
            //FLyraQuickBarSlotsChangedMessage Message;
            //Message.Owner = GetOwner();
            //Message.Slots = Slots;

            //UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
            //MessageSystem.BroadcastMessage(TAG_Lyra_QuickBar_Message_SlotsChanged, Message);
        }
    }

    return Result;
}