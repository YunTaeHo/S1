
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Bot/BoInterface.h"
#include "Misc/S1Container.h"
#include "S1BotCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);


/** foward declarations */
class US1PawnHandler;
class US1HealthComponent;
class UAbilitySystemComponent;
class UBehaviorTree;
class US1GameplayAbility;
class UAnimMontage;
class AS1PatrolRoute;


USTRUCT(BlueprintType)
struct FIdealRange
{
	GENERATED_BODY()

	FIdealRange() 
		: AttackRadius(600.f)
		, DefendRadius(600.f)
	{}

	UPROPERTY(EditAnywhere)
	float AttackRadius;
	UPROPERTY(EditAnywhere)
	float DefendRadius;
};

UCLASS()
class S1_API AS1BotCharacter : public AModularCharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AS1BotCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * ACharacter`s Interface
	 */
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


/** Behavior Tree 관련 기본 상태(모든 Bot 들이 사용할 수 있도록 설정, 안쓸 거면 안써도 된다) */
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(EMovementState MoveState) {}
	UFUNCTION(BlueprintCallable)
	virtual void DamageOnEvent(float Damage, TSubclassOf<UGameplayEffect> DamageEffect, AActor* DamageCursor = nullptr, EHitResponse HitResponse = EHitResponse::None);
	UFUNCTION(BlueprintCallable)
	virtual void JumpToDestination(FVector Location);

	/*
	 * 각자 공격을 Blueprint에서 함수를 여러 개 묶어서 사용할 수 있도록 설정 
	 *  - Boss의 Attack Skill이 4개라면(블루프린트에서 4개를 어떻게 처리할 지 설정)
	 *	- 각각의 보스 패턴들은 C++로 만들어 BlueprintCallable 하거나, 블루프린트로 구현
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack();

	/*
	 * 각자 죽음에 대해 처리
	 *	- Boos는 시네마틱을 처리할 수도 있음
	 *  - 기본 몬스터는 바로 죽게 만들어줌
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Die();

	/*
	 *	Hit 반응에 대해 처리
	 *  - 슈퍼아머를 지니고 있는 캐릭터는 스턴이나 넉백에 반응하지 않음
	 *	- 보스 캐릭터는 모두 상태에 대해 반응하지 않음
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HitReact(EHitResponse HitResponse);



	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;


protected:
	UPROPERTY(EditAnywhere, Category = "S1|Patrol")
	TObjectPtr<AS1PatrolRoute> PatrolRoute;

public:
	UFUNCTION(BlueprintCallable)
	AS1PatrolRoute* GetPatrolRoute() { return PatrolRoute; }

	UBehaviorTree* GetBehaviorTree() const { return BTAsset; }
	FIdealRange GetIdealRange() { return IdealRange; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() { return bDead; }

	/** 나중에 보스가 죽으면 모두 죽을 수 있도록 처리해야함. */
	void SetDead(bool InDead) { bDead = InDead; }


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
    TObjectPtr<US1HealthComponent> HealthComponent;

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Behavior")
	TObjectPtr<UBehaviorTree> BTAsset;


protected:
	/** BT : 공격, 방어 탐지에 대한 변수 */
	UPROPERTY(EditAnywhere, Category = "S1|Interface")
	FIdealRange IdealRange;

	/** 이동속도 관련 변수 (GetMovementSpeed 에서 사용) */
	UPROPERTY(EditAnywhere, Category = "S1|Movement")
	FMovementState MovementState;


protected:
	/** Nav Link 사용 시 점프 Z Velocity 값 */
	UPROPERTY(EditAnywhere, Category = "S1|AI|Jump")
	float ZVelocity = 250.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "S1|Dead")
	bool bDead;
}; 
