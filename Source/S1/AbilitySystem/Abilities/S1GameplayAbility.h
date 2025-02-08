
// Copyright (c) S1

#pragma once

#include "Abilities/GameplayAbility.h"
#include "S1GameplayAbility.generated.h"

/** foward declarations */
class US1AbilityCost;

UENUM(BlueprintType)
enum class ES1AbilityActivationPolicy : uint8
{
    /** Input이 Trigger 되었을 경우(Pressed/Released) */
    OnInputTriggered,
    /** Input이 Held되어 있을 경우 */
    WhileInputActive,
    /** avatar가 생성되었을 경우 바로 할당*/
    OnSpawn,
};

UCLASS()
class S1_API US1GameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    US1GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    /*
     * UGameplayAbility`s Interface 
     */
    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

    /** GA 활성화 정책(언제 활성화 될지?) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|AbilityActivation")
    ES1AbilityActivationPolicy ActivationPolicy;

    /** GameplayAbility가 Ability Cost를 사용해 Cost 관리 할 수 있도록 배열로 설정 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|AbilityActivation")
    TArray<TObjectPtr<US1AbilityCost>> AdditionalCosts;

}; 
