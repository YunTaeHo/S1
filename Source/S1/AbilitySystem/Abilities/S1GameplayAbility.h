
// Copyright (c) S1

#pragma once

#include "Abilities/GameplayAbility.h"
#include "S1GameplayAbility.generated.h"

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

UCLASS(Abstract)
class S1_API US1GameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    US1GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    /** GA 활성화 정책(언제 활성화 될지?) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|AbilityActivation")
    ES1AbilityActivationPolicy ActivationPolicy;
}; 
