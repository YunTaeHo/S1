// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatStatics.generated.h"

/** foward declarations */
class APawn;
class US1HealthSet;
class UAbilitySystemComponent;
struct FDamageInfo;

/**
 * 
 */
UCLASS()
class S1_API UCombatStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	/** S1HealthSet을 반환하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static const US1HealthSet* GetHealthSet(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float GetHealth(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float GetHealthNormalized(AActor* Actor);
	
	/** percent는 0 ~ 1 비율로 맞춰야함 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float GetHealPercentage(AActor* Actor, float Percent);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	static void ApplyDamageToTarget(AActor* Actor, AActor* Target, FDamageInfo DamageInfo);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	static void ApplyDamageToSelf(AActor* Actor, FDamageInfo DamageInfo);

private:
	static UAbilitySystemComponent* GetAbilitySystem(AActor* Actor);

};
