
// Copyright (c) S1

#include "S1AbilitySet.h"
#include "S1AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/S1GameplayAbility.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1AbilitySet)

US1AbilitySet::US1AbilitySet(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void FS1AbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
    if (Handle.IsValid())
    {
        AbilitySpecHandles.Add(Handle);
    }
}

void FS1AbilitySet_GrantedHandles::TakeFromAbilitySystem(US1AbilitySystemComponent* S1ASC)
{
    if (!S1ASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
    {
        if (Handle.IsValid())
        {
            // ActivatableAbilties에서 제거한다(ClearAbility)
            S1ASC->ClearAbility(Handle);
        }
    }
}

void US1AbilitySet::GiveToAbilitySystem(US1AbilitySystemComponent* S1ASC, FS1AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
    check(S1ASC);

    if (!S1ASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    // Gameplay Abilities를 허용:
    for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
    {
        const FS1AbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
        if (!IsValid(AbilityToGrant.Ability))
        {
            continue;
        }

        // GiveAbility()에서 EGameplayAbilityInstanctingPolicy::InstancedPerActor라면, 
        //  - 내부적으로 Instance를 생성하지만, 그렇지 않으면 CDO를 할당한다
        // 이를 통해 UObject 갯수를 줄여 UObject로 인한 성능적/메모리적 부하를 줄일 수 있다
        US1GameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<US1GameplayAbility>();

        // AbilitySpec은 GiveAbility로 전달되어, ActivatableAbilties에 추가된다
        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
        AbilitySpec.SourceObject = SourceObject;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle AbilitySpecHandle = S1ASC->GiveAbility(AbilitySpec);
        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
        }
    }
}


