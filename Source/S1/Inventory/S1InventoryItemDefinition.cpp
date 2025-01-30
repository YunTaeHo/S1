
// Copyright (c) S1

#include "S1InventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1InventoryItemDefinition)

US1InventoryItemDefinition::US1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

const US1InventoryItemFragment* US1InventoryItemDefinition::FindFragmentByClass(TSubclassOf<US1InventoryItemFragment> FragmentClass) const
{
    if (FragmentClass)
    {
        // Fragment를 순회하며, IsA()를 통해 해당 클래스를 가지고 있는지 확인
        for (US1InventoryItemFragment* Fragment : Fragments)
        {
            if (Fragment && Fragment->IsA(FragmentClass))
            {
                return Fragment;
            }
        }
    }
    return nullptr;
}


