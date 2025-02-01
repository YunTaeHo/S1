
// Copyright (c) S1

#pragma once

#include "S1AbilityCost.h"
#include "GameplayTagContainer.h"
#include "S1AbilityCost_ItemTagStack.generated.h"

/*
 * ItemInstance에서 해당 수량에 따라 행동 수행을 할 수 있는 지 결정하는 클래스
 */
UCLASS(meta = (DisPlayName = "Item Tag Stack"))
class S1_API US1AbilityCost_ItemTagStack : public US1AbilityCost
{
    GENERATED_BODY()

public:
    US1AbilityCost_ItemTagStack();

    virtual bool CheckCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
    virtual void ApplyCost(const US1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

    /*
     * 얼만큼 사용할 것인가에 대한 float값
     *  - FScalableFloat 은 float value로서 Curve로 Scale값을 조정할 수 있다
     *  - Curve는 Level에 따라 변경될 수 있다
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
    FScalableFloat Quantity;

    /** cost와 결합되는 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
    FGameplayTag Tag;

    /** 태그와 연결이 실패했을 때 발동되는 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Costs")
    FGameplayTag FailureTag;

}; 
