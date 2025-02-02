
// Copyright (c) S1

#include "S1DamageExecution.h"
#include "AbilitySystem/Attributes/S1CombatSet.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1DamageExecution)


/*
 * 해당 Struct를 사용하여, FGameplayEffectAttributeCaptureDefinition 인스턴스화하여 전달한다
 */
struct FDamageStatics
{
	/** AttributeSet의 어떤 Attribute를 Capture할 것인지와 어떻게 Capture할지 정의를 담고 있다 */
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(US1CombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

US1DamageExecution::US1DamageExecution()
	: Super()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}


void US1DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// GameplayEffectSpec은 GameplayEffect의 핸들이다
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	

	float BaseDamage = 0.f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		// 해당 함수 호출을 통해 S1CombatSet의 BaseDamage 값을 가져온다 (혹은 값이 Modifier에 누적되어 있었다면, 최종 계산 결과가 나온다)
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);
	}

	// @ TODO HORK 거리 비례 데미지 추가해야하나? RangeWeapon에 그렇다면 Interface만들어주자
	//	이후 재질(특성) 당 데미지를 다르게 줘야할 듯 싶다



	// RelevantAttributesToCapture를 통해 최종 계산된 BaseDamage을 0.0이하가 되지 않도록 한다
	const float DamageBuff = Spec.GetLevel();

	// Head는 1.5배, Body는 1배로 데미지를 입혀주자
	const float DamageDone = DamageBuff > KINDA_SMALL_NUMBER 
		? FMath::Max(0.f, BaseDamage * DamageBuff) 
		: FMath::Max(0.f, DamageBuff);

	if (DamageDone > 0.f)
	{
		// GameplayEffectCalculation 이후, Modifier로서 추가한다 : 
		// - 해당 Mdifier는 CombatSet에서 가져온 BaseHeal을 활용하여, HealthSet의 healing에 추가해준다
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(US1HealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
}


