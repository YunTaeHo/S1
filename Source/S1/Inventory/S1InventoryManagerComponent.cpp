
// Copyright (c) S1

#include "S1InventoryManagerComponent.h"
#include "S1InventoryItemInstance.h"
#include "S1InventoryItemDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(S1InventoryManagerComponent)

US1InventoryManagerComponent::US1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , InventoryList(this)
{

}

US1InventoryItemInstance* FS1InventoryList::AddEntry(TSubclassOf<US1InventoryItemDefinition> ItemDef)
{
    US1InventoryItemInstance* Result = nullptr;
    check(ItemDef);
    check(OwnerComponent);

    AActor* OwningActor = OwnerComponent->GetOwner();
    check(OwningActor->HasAuthority());

    FS1InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Instance = NewObject<US1InventoryItemInstance>(OwningActor);
    NewEntry.Instance->ItemDef = ItemDef;

    // Fragments를 순회하며(반복자)
    for (US1InventoryItemFragment* Fragment : GetDefault<US1InventoryItemDefinition>(ItemDef)->Fragments)
    {
        if (Fragment)
        {
            // OnInstanceCreated에 Instance된 객체를 인자로 넣어준다
            Fragment->OnInstanceCreated(NewEntry.Instance);
        }
    }

    Result = NewEntry.Instance;
    return Result;
}


US1InventoryItemInstance* US1InventoryManagerComponent::AddItemDefinition(TSubclassOf<US1InventoryItemDefinition> ItemDef)
{
    US1InventoryItemInstance* Result = nullptr;
    if (ItemDef)
    {
        Result = InventoryList.AddEntry(ItemDef);
    }

    return Result;
}

