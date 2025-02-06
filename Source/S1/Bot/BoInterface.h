// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBoInterface : public UInterface
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FIdealRange
{
	GENERATED_BODY()

	FIdealRange() {}

	float AttackRadius = 0.f;
	float DefendRadius = 0.f;
};

class UGameplayEffect;
class AActor;

/**
 * 
 */
class S1_API IBoInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetPatrolRoute() {};
	virtual void SetMovementSpeed() {};
	virtual FIdealRange GetIdealRange() { return FIdealRange(); }
	virtual void EquipWeapon() {};
	virtual void UnequipWeapon() {};
	virtual void Attack() {};
	virtual void DamageOnEvent(float Damage, TSubclassOf<UGameplayEffect> DamageEffect, AActor* DamageCursor) {};
	virtual void JumpToDestination(FVector Location) {}; 
	virtual void Die() {};

};
