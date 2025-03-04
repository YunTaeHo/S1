﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStatics.h"
#include "S1CombatSystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include "AbilitySystemGlobals.h"
#include "Perception/AISense_Damage.h"
#include "GameFramework/Character.h"
#include "Character/S1Character.h"
#include "Character/S1BotCharacter.h"
#include "Bullet/S1Bullet.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	if (ICombatInterface* Character = Cast<ICombatInterface>(Actor))
	{
		if (GetAbilitySystem(Actor))
		{
			UAISense_Damage::ReportDamageEvent(Actor, Target, Actor, DamageInfo.DamageAmount, Actor->GetActorLocation(), Target->GetActorLocation());
			Character->DamageOnEvent(Target, DamageInfo);
		}
	}
}


void UCombatStatics::ApplyDamageToSelf(AActor* Actor, FDamageInfo DamageInfo)
{
	if (ICombatInterface* Character = Cast<ICombatInterface>(Actor))
	{
		if (GetAbilitySystem(Actor))
		{
			Character->DamageOnEvent(Actor, DamageInfo);
		}
	}
}

TArray<AActor*> UCombatStatics::DamageAllNonTeamMembers(AActor* Owner, const TArray<FHitResult>& Hits, FDamageInfo Info)
{
	TArray<AActor*> ActorsDamagedSoFar;

	// Hits는 순회하며
	for (const FHitResult& Hit : Hits)
	{
		// 충돌된 액터가 존재하고
		if (AActor* HitActor = Hit.GetActor())
		{
			// 현재 충돌하지 않은 상태에서 && 서로 다른 팀이고 && 무적이 아니라면?
			if (AnotherTeamNumber(HitActor, Owner) && !ActorsDamagedSoFar.Contains(HitActor) && !IsInvis(HitActor))
			{
				// 데미지를 주고 배열에 추가한다
				ApplyDamageToTarget(Hit.GetActor(), Owner, Info);
				ActorsDamagedSoFar.AddUnique(HitActor);
			}
		}
	}

	return ActorsDamagedSoFar;
}

AActor* UCombatStatics::DamageFirstNonTeamMember(AActor* Owner, const TArray<FHitResult>& Hits, FDamageInfo Info)
{
	// Hits는 순회하며
	for (const FHitResult& Hit : Hits)
	{
		// 충돌된 액터가 존재하고
		if (AActor* HitActor = Hit.GetActor())
		{
			// 서로 다른팀이고 && 무적이 아니라면?
			if (AnotherTeamNumber(HitActor, Owner) && !IsInvis(HitActor))
			{
				// 데미지를 주고 바로 반환한다
				ApplyDamageToTarget(Hit.GetActor(), Owner, Info);
				return HitActor;
			}
		}
	}

	return nullptr;
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

bool UCombatStatics::AnotherTeamNumber(AActor* Source, AActor* Target)
{
	// Combat과 관련된 인터페이스가 있으며
	if (ITeamInterface* OtherActor = Cast<ITeamInterface>(Target))
	{
		// Owner도 해당 인터페이스가 있고
		if (ITeamInterface* OwnerActor = Cast<ITeamInterface>(Source))
		{
			// 현재 충돌하지 않은 상태에서 서로 다른 팀이라면?
			if (OwnerActor->GetTeamNumber() != OtherActor->GetTeamNumber())
			{
				return true;
			}
		}
	}

	return false;
}

void UCombatStatics::SpawnLinearProjectile(FTransform SpawnTransform, TSubclassOf<AS1Bullet> BulletFactory, AActor* Owner, AActor* Target, FDamageInfo DamageInfo, FBulletInfo BulletInfo)
{
	if (Owner && BulletFactory)
	{
		BulletInfo.Target = Target;
		AS1Bullet::SpawnLinear(Owner, SpawnTransform, BulletFactory, DamageInfo, BulletInfo);
	}
}

void UCombatStatics::SphereTraceDamage(AActor* WorldContextObject, FVector Start, FVector End, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, TArray<AActor*> ActorToIgnore, FDamageInfo DamageInfo, bool bIgnoreSelf)
{
	if (WorldContextObject)
	{
		TArray<FHitResult> Hits;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects
		(
			WorldContextObject->GetWorld(),
			Start,
			End,
			Radius,
			ObjectTypes,
			false,
			ActorToIgnore,
			EDrawDebugTrace::Type::None,
			Hits,
			bIgnoreSelf
		))
		{
			UCombatStatics::DamageAllNonTeamMembers(WorldContextObject, Hits, DamageInfo);
		}
	}
}

void UCombatStatics::JumpToVelocity(AActor* Owner, FVector StartPos, FVector EndPos, float ZVelocity, bool XYOverride, bool ZOverride)
{
	EndPos.Z += ZVelocity;

	FVector LaunchVelocity;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(Owner->GetWorld(), LaunchVelocity, StartPos, EndPos);
	
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		Character->LaunchCharacter(LaunchVelocity, XYOverride, ZOverride);
	}
}

bool UCombatStatics::IsInvis(AActor* HitActor)
{
	if (!HitActor)
	{
		return false;
	}

	if (AModularCharacter* Character = Cast<AModularCharacter>(HitActor))
	{
		// 무적 상태라면?(구르기) 공격 당하지 말자
		if (Character->IsInvis())
		{
			return true;
		}
	}

	return false;
}

