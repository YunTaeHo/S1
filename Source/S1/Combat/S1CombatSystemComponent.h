﻿
// Copyright (c) S1

#pragma once

#include "Components/PawnComponent.h"
#include "MIsc/S1Container.h"
#include "S1CombatSystemComponent.generated.h"

/** 캐릭터마다 함수들을 재정의해 바인딩해서 서로 다른 상태를 수행하도록 하자 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitReacted, bool, bHitReacted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlocked, EBlockingState, BlockState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaggered, bool, bStaggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStuned, bool, bStuned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKnockBacked, bool, bKnockBacked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttacked, bool, bAttacked, EAttackType, AttackType);


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class S1_API US1CombatSystemComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    US1CombatSystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(BlueprintAssignable)
    FHitReacted OnHitReacted;

    UPROPERTY(BlueprintAssignable)
    FOnBlocked OnBlocked;

    UPROPERTY(BlueprintAssignable)
    FOnStaggered OnStaggered;

    UPROPERTY(BlueprintAssignable)
    FOnStuned OnStuned;

    UPROPERTY(BlueprintAssignable)
    FOnKnockBacked OnKnockBacked;

    UPROPERTY(BlueprintAssignable)
    FOnAttacked OnAttacked;

public:
    bool IsBlocked() { return bIsBlocked; }
    void SetBlock(bool bBlock) { bIsBlocked = bBlock; }
   
    bool IsAttacked() { return bIsAttacked; }
    void SetAttack(bool bAttack) { bIsBlocked = bAttack; }

    bool IsInterrupt() { return bIsInterrupt; }
    void SetInterrupt(bool bInterrupt) { bIsInterrupt = bInterrupt; }

protected:
    /** 현재 기본 공격을 맞은 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsHitReacted;

    /** 현재 막는 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsBlocked;

    /** 현재 방해받지 않는 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsInterrupt;

    /** 현재 그로기 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsStaggered;

    /** 현재 스턴 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsStuned;

    /** 현재 넉백 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsKnockBacked;

    /** 현재 공격 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsAttacked;
   
    /** 사망 상태인지? */
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    bool bIsDead;

public:
    bool IsDead() { return bIsDead; }
    void SetDead(bool bDead) { bIsDead = bDead; }

public:
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool ReserveAttackToken(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ReturnAttackToken(int32 Amount);

public:
    int32 GetTeamNumber() const { return TeamNumber; }

protected:
    UPROPERTY(BlueprintReadWrite, Category = "S1")
    int32 AttackTokenCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "S1")
    FDamageInfo DamageInfos;

    UPROPERTY(EditAnywhere, Category = "S1")
    int32 TeamNumber;

}; 
