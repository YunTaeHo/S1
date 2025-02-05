﻿
// Copyright (c) S1

#include "S1HealExecution.h"
#include "AbilitySystem/Attributes/S1CombatSet.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1HealExecution)

/*
 * 해당 Struct를 사용하여, FGameplayEffectAttributeCaptureDefinition 인스턴스화하여 전달한다
 */
struct FHealStatics
{
	/** AttributeSet의 어떤 Attribute를 Capture할 것인지와 어떻게 Capture할지 정의를 담고 있다 */
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FHealStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(US1CombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealStatics& HealStatics()
{
	// 계속 FHealStatics를 생성하는 것은 부하이니, 한번만 생성하고 재사용한다
	static FHealStatics Statics;
	return Statics;
}

US1HealExecution::US1HealExecution()
	: Super()
{
	// Source로 (입력값) Attribute의 캡처를 정의하자
	//	- CombatSet::BaseHeal을 통해 Healing 값을 정의하고,
	//	  최종 Execute할 때 해당 값을 가져와서 Health에 Healing을 적용한다
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
}

void US1HealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// GameplayEffectSpec은 GameplayEffect의 핸들이다
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float BaseHeal = 0.f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		// 해당 함수 호출을 통해 S1CombatSet의 BaseHeal 값을 가져온다 (혹은 값이 Modifier에 누적되어 있었다면, 최종 계산 결과가 나온다)
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);
	}

	const float HealtheBuff = Spec.GetLevel();

	// RelevantAttributesToCapture를 통해 최종 계산된 BaseHeal을 0.0이하가 되지 않도록 한다
	const float HealingDone = FMath::Max(0.f, BaseHeal + HealtheBuff);
	if (HealingDone > 0.f)
	{
		// GameplayEffectCalculation 이후, Modifier로서 추가한다 : 
		// - 해당 Mdifier는 CombatSet에서 가져온 BaseHeal을 활용하여, HealthSet의 healing에 추가해준다
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(US1HealthSet::GetHealingAttribute(), EGameplayModOp::Additive, HealingDone));
	}
}
