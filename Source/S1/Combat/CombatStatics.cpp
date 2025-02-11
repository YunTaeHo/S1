// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStatics.h"
#include "S1CombatSystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include "AbilitySystemGlobals.h"
#include "Character/S1Character.h"
#include "Character/S1BotCharacter.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

const US1HealthSet* UCombatStatics::GetHealthSet(AActor* Actor)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystem(Actor))
	{
		if (const US1HealthSet* HealthSet = ASC->GetSet<US1HealthSet>())
		{
			return HealthSet;
		}
	}

	return nullptr;
}

float UCombatStatics::GetHealth(AActor* Actor)
{
	const US1HealthSet* HealthSet = GetHealthSet(Actor);
	if (!HealthSet)
	{
		return 0.f;
	}

	return HealthSet->GetHealth();
}

float UCombatStatics::GetHealthNormalized(AActor* Actor)
{
	const US1HealthSet* HealthSet = GetHealthSet(Actor);
	if (!HealthSet)
	{
		return 0.f;
	}


	return HealthSet->GetHealth() / HealthSet->GetMaxHealth();
}

float UCombatStatics::GetHealPercentage(AActor* Actor, float Percent)
{
	const US1HealthSet* HealthSet = GetHealthSet(Actor);
	if (!HealthSet)
	{
		return 0.f;
	}

	return HealthSet->GetMaxHealth() * FMath::Clamp(Percent, 0.f, 1.f);
}

void UCombatStatics::ApplyDamageToTarget(AActor* Actor, AActor* Target, FDamageInfo DamageInfo)
{
	if (ICombatInterface* Player = Cast<AS1Character>(Actor))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystem(Actor))
		{
			Player->DamageOnEvent(Target, DamageInfo);
		}
	}
}


void UCombatStatics::ApplyDamageToSelf(AActor* Actor, FDamageInfo DamageInfo)
{
	if (ICombatInterface* Player = Cast<AS1Character>(Actor))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystem(Actor))
		{
			Player->DamageOnEvent(Actor, DamageInfo);
		}
	}
}

UAbilitySystemComponent* UCombatStatics::GetAbilitySystem(AActor* Actor)
{
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!ASC)
	{
		FString PawnName = TEXT("Have not ASC : ") + Actor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, PawnName);
		return nullptr;
	}

	return ASC;
}

