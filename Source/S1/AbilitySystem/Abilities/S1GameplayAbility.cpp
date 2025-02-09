
// Copyright (c) S1

#include "S1GameplayAbility.h"
#include "S1AbilityCost.h"
#include "AbilitySystemComponent.h"
#include "Character/S1PlayerComponent.h"
#include "Player/S1PlayerController.h"
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

//PRAGMA_DISABLE_OPTIMIZATION
//bool US1GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
//{
//    if (ActivationBlockedTags.Num() || ActivationRequiredTags.Num())
//    {
//        static FGameplayTagContainer AbilitySystemComponentTags;
//        AbilitySystemComponentTags.Reset();
//
//        
//        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
//        ASC->GetOwnedGameplayTags(AbilitySystemComponentTags);
//
//        if (AbilitySystemComponentTags.HasAny(ActivationOwnedTags))
//        {
//            return false;
//        }
//
//        if (!ActivationOwnedTags.HasAll(AbilitySystemComponentTags))
//        {
//            return false;
//        }
//    }
//
//    return true;
//}
//PRAGMA_ENABLE_OPTIMIZATION

void US1GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    ClearCameraMode();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

US1PlayerComponent* US1GameplayAbility::GetS1CharacterFromActorInfo() const
{
    return (CurrentActorInfo ? US1PlayerComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void US1GameplayAbility::SetCameraMode(TSubclassOf<US1CameraMode> CameraMode)
{
    if (US1PlayerComponent* PlayerComponent = GetS1CharacterFromActorInfo())
    {
        PlayerComponent->SetAbilityCameraMode(CameraMode, CurrentSpecHandle);
        ActiveCameraMode = CameraMode;
    }
}


void US1GameplayAbility::ClearCameraMode()
{
    if (ActiveCameraMode)
    {
        if (US1PlayerComponent* PlayerComponent = GetS1CharacterFromActorInfo())
        {
            PlayerComponent->ClearAbilityCameraMode(CurrentSpecHandle);
        }

        ActiveCameraMode = nullptr;
    }
}
