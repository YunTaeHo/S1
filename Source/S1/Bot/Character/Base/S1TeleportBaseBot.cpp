
// Copyright (c) S1

#include "S1TeleportBaseBot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1TeleportBaseBot)

AS1TeleportBaseBot::AS1TeleportBaseBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1TeleportBaseBot::Teleport(FVector Location)
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->SetMovementMode(EMovementMode::MOVE_Flying);
    Movement->MaxFlySpeed = 5000.f;
    Movement->MaxAcceleration = 99999.f;

    GetMesh()->SetVisibility(false, true);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
    TeleporStart(Location);
}

void AS1TeleportBaseBot::Attack(AActor* Target)
{ 
    Super::Attack(Target);

    if (bCanAttack)
    {
        PlayAttackMontage(Target);
    }
}

void AS1TeleportBaseBot::TeleporEnd()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->SetMovementMode(EMovementMode::MOVE_Walking);
    Movement->MaxAcceleration = 1500.f;

    GetMesh()->SetVisibility(true, true);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
    DestroyedEffect();
    OnTeleportEnd.Broadcast();
}


