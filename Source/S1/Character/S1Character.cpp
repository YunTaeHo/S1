// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Character.h"
#include "S1PawnHandler.h"
#include "Camera/S1CameraComponent.h"
#include "S1HealthComponent.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "Combat/S1CombatSystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1Character)

AS1Character::AS1Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//S1PawnHandler 설정
	PawnHandlerComponent = CreateDefaultSubobject<US1PawnHandler>(TEXT("PawnHandler"));
	{
		PawnHandlerComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &AS1Character::OnAbilitySystemInitialized));
		PawnHandlerComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &AS1Character::OnAbilitySystemUninitialized));
	}

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<US1CameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0, 0.0, 75.0));
	}
	// HealthComponent 생성
	{
		HealthComponent = CreateDefaultSubobject<US1HealthComponent>(TEXT("HealthComponent"));
	}

	// CombatSystemComponent 생성
	{
		CombatSystemComponent = CreateDefaultSubobject<US1CombatSystemComponent>(TEXT("CombatSystemComponent"));
	}

	
}

void AS1Character::OnAbilitySystemInitialized()
{
	US1AbilitySystemComponent* S1ASC = Cast<US1AbilitySystemComponent>(GetAbilitySystemComponent());
	check(S1ASC);

	// HealthComponent의 ASC를 통한 초기화
	HealthComponent->InitializeWithAbilitySystem(S1ASC);
}

void AS1Character::OnAbilitySystemUninitialized()
{
	HealthComponent->UnInitializeWithAbilitySystem();
}

void AS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess가 되었기 때문에 Controller와 PlayerState 정보 접근이 가능한 상태
	PawnHandlerComponent->ForceUpdateInitState();
}

UAbilitySystemComponent* AS1Character::GetAbilitySystemComponent() const
{
	// 캐싱되어있는 ASC를 가져와서 return 해준다
	return PawnHandlerComponent->GetS1AbilitySystemComponent();
}

void AS1Character::DamageOnEvent(AActor* DamageCursor, FDamageInfo Info)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);

	if (Info.DamageEffect)
	{
		// 기존 Health를 저장하고
		const US1HealthSet* HealthSet = ASC->GetSet<US1HealthSet>();
		float OldValue = HealthSet->GetHealth();

		// Damage를 적용한다
		FGameplayEffectContextHandle EffectContext;
		ASC->BP_ApplyGameplayEffectToSelf(Info.DamageEffect, Info.Level, EffectContext);

		// Damage가 적용된 Health를 가져온다
		float NewValue = HealthSet->GetHealth();

		// 이후 Health가 변경된 것을 확인할 수 있게 Brocast를 호출한다
		HealthComponent->OnHealthChanged.Broadcast(HealthComponent, OldValue, NewValue, DamageCursor);

		// 아직 살아있다면 HitReact를 넣어주고
		if (NewValue > 0.f)
		{
			HitReact(Info.HitResponse, DamageCursor);
		}
		// 죽었다면 Die를 호출한다
		else
		{
			CombatSystemComponent->SetDead(true);
			Die();
		}
	}
}

void AS1Character::TryToBlock()
{
}

void AS1Character::StartBlcok(EBlockingState BlockingState)
{
}

void AS1Character::Blocking(EBlockingState BlockingState)
{
}

void AS1Character::EndBlcok()
{
}

int32 AS1Character::GetTeamNumber()
{
	return CombatSystemComponent->GetTeamNumber();
}

bool AS1Character::IsDead()
{
	return CombatSystemComponent->IsDead();
}

void AS1Character::SetDead(bool bDead)
{
	CombatSystemComponent->SetDead(bDead);
}

bool AS1Character::ReserveAttackToken(int32 Amount)
{
	return CombatSystemComponent->ReserveAttackToken(Amount);
}

void AS1Character::ReturnAttackToken(int32 Amount)
{	
	CombatSystemComponent->ReturnAttackToken(Amount);
}

void AS1Character::HitReact(EHitResponse Respose, AActor* Target)
{
}

void AS1Character::Die()
{
}

void AS1Character::Attack(AActor* AttackTarget)
{
}
