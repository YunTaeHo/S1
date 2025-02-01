
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "S1GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "S1AbilityCost.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class S1_API US1AbilityCost : public UObject
{
    GENERATED_BODY()

public:
    US1AbilityCost();


    /*
     * CheckCost, ApplayCost 함수는 UGameAbility의 CheckCost와 ApplyCost이다
     *  - GaemplayAbility에 따라 다르게 설정해주면 된다
     */
    virtual bool CheckCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
    {
        return true;
    }

    virtual void ApplyCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
    {
    }

}; 
