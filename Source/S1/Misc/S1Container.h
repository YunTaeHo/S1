
// Copyright (c) S1

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "S1Container.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    Idle,
    Walking,
    Jogging,
    Sprinting,
};

USTRUCT(BlueprintType)
struct FMovementState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "S1|Movement")
    float IdleSpeed;

    UPROPERTY(EditAnywhere, Category = "S1|Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, Category = "S1|Movement")
    float JogSpeed;

    UPROPERTY(EditAnywhere, Category = "S1|Movement")
    float SprintSpeed;

};

UENUM(BlueprintType)
enum class EHitResponse : uint8
{
    None,
    HitReaction,
    Stagger,
    Stun,
    KnockBack
};

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
    Pistol,
    Rifle,
    RPG,
    Sword,
    Spear,
    Magic,
};



UCLASS()
class S1_API US1Container : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
}; 
