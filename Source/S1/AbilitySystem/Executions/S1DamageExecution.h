
// Copyright (c) S1

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "S1DamageExecution.generated.h"

UCLASS()
class S1_API US1DamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    US1DamageExecution();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

}; 
