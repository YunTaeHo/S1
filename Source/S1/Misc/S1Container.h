
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
    Frozen,
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

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Tracing,
    Melee,
    Projectile,
    Grenade,
    Magic,
};

/*
 * 가드 성공, 가드 파괴, 가드 성공했지만 넉백 등의 상태를 표현하는 Enum값
 * 공격에서 이러한 상태를 넘겨주도록 하자
 */
UENUM(BlueprintType)
enum class EBlockingState : uint8
{
    None,
    Block,
    BlockSuccessfully,
    BlockBreaken,
    BlockKnockBack,
};


UCLASS()
class S1_API US1Container : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
}; 
