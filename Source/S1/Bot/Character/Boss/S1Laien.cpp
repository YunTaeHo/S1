
// Copyright (c) S1

#include "S1Laien.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Bot/Controller/S1BotController.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1Laien)

AS1Laien::AS1Laien(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1Laien::Teleport(FVector Location)
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->SetMovementMode(EMovementMode::MOVE_Flying);
    Movement->MaxFlySpeed = 1600.f;
    Movement->MaxAcceleration = 4500.f;
    TeleporStart(Location, BotController->GetTargetEnemy());
}

void AS1Laien::TeleporEnd()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->SetMovementMode(EMovementMode::MOVE_Walking);
    Movement->MaxAcceleration = 1500.f;

    DestroyedEffect();
    OnTeleportEnd.Broadcast();
}