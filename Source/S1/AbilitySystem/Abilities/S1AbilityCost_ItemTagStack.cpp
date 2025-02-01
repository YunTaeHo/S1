
// Copyright (c) S1

#include "S1AbilityCost_ItemTagStack.h"
#include "NativeGameplayTags.h"
#include "Equipment/S1GameplayAbility_FromEquipment.h"
#include "Inventory/S1InventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1AbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "ABility.ActivateFail.Cost")

US1AbilityCost_ItemTagStack::US1AbilityCost_ItemTagStack()
    : Super()
{
    Quantity.SetValue(1.f);
    FailureTag = TAG_ABILITY_FAIL_COST;
}

bool US1AbilityCost_ItemTagStack::CheckCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
    // 장착한 아이템에 대해서만 수행한다 해당 cost를 수행한다
    if (const US1GameplayAbility_FromEquipment* EquipmentAbility = Cast<US1GameplayAbility_FromEquipment>(Ability))
    {
        if (US1InventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
        {
            const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

            // 현재, 무기를 샷건이라고 가정하면 
            // lv2 : 네발을 사용하고, 더욱 강력한 데메지
            // Lv5 : 열발을 사용하고, 최대 데미지
            // 이렇게 구분할 수 있기 때문에 해당 로직으로 작성한다
            const float NumStackReal = Quantity.GetValueAtLevel(AbilityLevel);
            const int32 NumStacks = FMath::TruncToInt(NumStackReal);
            const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

            // 해당 Ability가 실패했을 때, FailureTag를 넣어준다
            if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
            {
                OptionalRelevantTags->AddTag(FailureTag);
            }
            return bCanApplyCost;
        }
    }

    return false;
}

void US1AbilityCost_ItemTagStack::ApplyCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    if (const US1GameplayAbility_FromEquipment* EquipmentAbility = Cast<US1GameplayAbility_FromEquipment>(Ability))
    {
        if (US1InventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
        {
            const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);
            const float NumStackReal = Quantity.GetValueAtLevel(AbilityLevel);
            const int32 NumStacks = FMath::TruncToInt(NumStackReal);

            ItemInstance->RemoveStatTagStack(Tag, NumStacks);
        }
    }
}
