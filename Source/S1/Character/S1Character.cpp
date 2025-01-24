// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Character.h"
#include "S1PawnHandler.h"
#include "Camera/S1CameraComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1Character)

AS1Character::AS1Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//S1PawnHandler 설정
	PawnHandlerComponent = CreateDefaultSubobject<US1PawnHandler>(TEXT("PawnHandler"));

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<US1CameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0, 0.0, 75.0));
	}
}

void AS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess가 되었기 때문에 Controller와 PlayerState 정보 접근이 가능한 상태
	PawnHandlerComponent->ForceUpdateInitState();
}
