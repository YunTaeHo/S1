
// Copyright (c) S1

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "S1Container.generated.h"

/** foward declarations */
class UGameplayEffect;

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
    Unarmod,
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

UENUM(BlueprintType)
enum class ELandState : uint8
{
    NormalLand,
    SoftLand,
    HeavyLand,
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

/*
 *  Damage에 대한 모든 정보를 담아주는 구조체
 *  - 무기의 Level로 데미지를 올려준다
 *  - 해당 DamageEffect로 데미지를 적용시킨다
 *  - 기절, 그로기를 시킬 수 있다
 *  - Damage를 준 객체가 누군 지 알 수 있다
 */
USTRUCT(BlueprintType)
struct FDamageInfo
{
    GENERATED_BODY()

    FDamageInfo() {}

    FDamageInfo(const FDamageInfo& Other) 
        : Level(Other.Level)
        , DamageEffect(Other.DamageEffect)
        , DamageAmount(Other.DamageAmount)
        , HitResponse(Other.HitResponse)
        , bCanBlocked(Other.bCanBlocked)
        , bShouldForceInterrupt(Other.bShouldForceInterrupt)
    {}

    FDamageInfo(float InLevel, TSubclassOf<UGameplayEffect> InDamageEffect, float InDamageAmount, EHitResponse InHitResponse = EHitResponse::None)
        : Level(InLevel)
        , DamageEffect(InDamageEffect)
        , HitResponse(InHitResponse)
    {}

    /** GameplayEffect에 대한 Level */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    float Level;

    /** 공격을 수행할 GameplayEffect */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** AI Perception에서 Damage의 반경 범위를 지정 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    float DamageAmount;

    /** 공격 타입이 뭔지 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    EHitResponse HitResponse;

    /** 방어가 가능한 지 확인 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    bool bCanBlocked;

    /** 강제로 공격을 입힐지 확인 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    bool bShouldForceInterrupt;

};



/*
 *  Bullet에 대한 모든 정보를 담아주는 구조체
 *  - 무기의 Level로 데미지를 올려준다
 *  - 해당 DamageEffect로 데미지를 적용시킨다
 *  - 기절, 그로기를 시킬 수 있다
 *  - Damage를 준 객체가 누군 지 알 수 있다
 */
USTRUCT(BlueprintType)
struct FBulletInfo
{
    GENERATED_BODY()

    FBulletInfo() {}

    FBulletInfo(float InSpeed, float InGravity, AActor* InTarget = nullptr, bool bHoming = false, AActor* InInstigator = nullptr)
        : Speed(InSpeed)
        , Gravity(InGravity)
        , Target(InTarget)
        , bIsHoming(bHoming)
        , Instigator(InInstigator)
    {
    }

    /** 투사체 속도 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    float Speed;

    /** 투사체 중력 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    float Gravity;

    /** 타겟을 저장 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    AActor* Target;

    /** 유도탄인지 확인 */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    bool bIsHoming;

    /** 내가 발사한 객체는 누군지? */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    AActor* Instigator;

    /** 몇 번 팀읹? */
    UPROPERTY(EditAnywhere, Category = "S1|Damage")
    int32 TeamNumber;
    
};

UCLASS()
class S1_API US1Container : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
}; 
