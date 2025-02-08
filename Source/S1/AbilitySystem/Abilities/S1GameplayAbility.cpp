
// Copyright (c) S1

#include "S1GameplayAbility.h"
#include "S1AbilityCost.h"
#include "AbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameplayAbility)

US1GameplayAbility::US1GameplayAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ActivationPolicy = ES1AbilityActivationPolicy::OnInputTriggered;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool US1GameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) || !ActorInfo)
    {
        return false;
    }

    // 내가 넣어준 AbilityCost를 모두 순회한다
    for (TObjectPtr<US1AbilityCost> AdditionalCost : AdditionalCosts)
    {
        if (AdditionalCost != nullptr)
        {
            // 만약 Cost중 하나가 CheckCost를 실패한다면 해당 GA는 작동하지 않는다
            if (!AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
            {
                return false;
            }
        }
    }

    // 모든 AbilityCost가 된다면 true로 반환
    return true;
}

void US1GameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
    check(ActorInfo);

    // 내가 넣어준 AbilityCost를 모두 순회하면서 Cost 감소
    for (TObjectPtr<US1AbilityCost> AdditionalCost : AdditionalCosts)
    {
        if (AdditionalCost != nullptr)
        {
            AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
        }
    }
}

bool US1GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (ActivationBlockedTags.Num() || ActivationRequiredTags.Num())
    {
        static FGameplayTagContainer AbilitySystemComponentTags;
        AbilitySystemComponentTags.Reset();

        
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        ASC->GetOwnedGameplayTags(AbilitySystemComponentTags);

        if (ActivationOwnedTags.HasAny(ActivationBlockedTags))
        {
            return false;
        }

        if (!ActivationOwnedTags.HasAll(ActivationRequiredTags))
        {
            return false;
        }
    }

    return true;
}
