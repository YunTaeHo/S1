// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Character.h"
#include "S1PawnHandler.h"
#include "Camera/S1CameraComponent.h"
#include "S1HealthComponent.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "Combat/S1CombatSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1Character)

AS1Character::AS1Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//S1PawnHandler 설정
	PawnHandlerComponent = CreateDefaultSubobject<US1PawnHandler>(TEXT("PawnHandler"));
	{
		PawnHandlerComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
		PawnHandlerComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
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

bool AS1Character::ReserveAttackToken(int32 Amount)
{
	if (US1CombatSystemComponent* CombatSystem = FindComponentByClass<US1CombatSystemComponent>())
	{
		return CombatSystem->ReserveAttackToken(Amount);
	}

	return false;
}

void AS1Character::ReturnAttackToken(int32 Amount)
{
	if (US1CombatSystemComponent* CombatSystem = FindComponentByClass<US1CombatSystemComponent>())
	{
		CombatSystem->ReturnAttackToken(Amount);
	}
}
