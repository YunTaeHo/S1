
// Copyright (c) S1

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "S1HealExecution.generated.h"

/*
 * US1HealExecution은 GameplayEffect의 Execution을 사용자 정의에 따라 GameplayEffect를 처리할 숭 ㅣㅆ다 : 
 *  - HealExecution은 HealAttribute의 Healing에 적용된다
 */
UCLASS()
class S1_API US1HealExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    US1HealExecution();

    /** 해당 메서드는 GameplayEffectExecutionCalculation의 Execute() BlueprintNativeEvent를 오버라이드 한다*/
    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

}; 
