
// Copyright (c) S1

#include "S1InventoryItemInstance.h"
#include "S1InventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1InventoryItemInstance)

US1InventoryItemInstance::US1InventoryItemInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

const US1InventoryItemFragment* US1InventoryItemInstance::FindFragmentByClass(TSubclassOf<US1InventoryItemFragment> FragmentClass) const
{
    if ((ItemDef != nullptr) && (FragmentClass != nullptr))
    {
        // Fragment 정보를 Definition에서 가져와 반환해주자
        return GetDefault<US1InventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
    }

    return nullptr;
}

void US1InventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
    StatTags.AddStack(Tag, StackCount);
}

void US1InventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
    StatTags.RemoveStack(Tag, StackCount);
}

bool US1InventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
    return StatTags.ContainsTag(Tag);
}

int32 US1InventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
    return StatTags.GetStackCount(Tag);
}


