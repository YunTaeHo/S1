
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Misc/S1Container.h"
#include "Interface/CombatInterface.h"
#include "S1BotCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlockEnded);


/** foward declarations */
class US1PawnHandler;
class US1HealthComponent;
class UAbilitySystemComponent;
class UBehaviorTree;
class US1GameplayAbility;
class UAnimMontage;
class AS1PatrolRoute;
class US1BotCombatSystemComponent;
struct FDamageInfo;


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
class S1_API AS1BotCharacter : public AModularCharacter, public IAbilitySystemInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    AS1BotCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * ACharacter`s Interface
	 */
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/*
	 * ICombatInterface`s Interface
	 */
	virtual void DamageOnEvent(AActor* DamageCursor, FDamageInfo Info) override;



/** Behavior Tree 관련 기본 상태(모든 Bot 들이 사용할 수 있도록 설정, 안쓸 거면 안써도 된다) */
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(EMovementState MoveState) {}

	UFUNCTION(BlueprintCallable)
	virtual void JumpToDestination(FVector Location);

	/*
	 * 각자 공격을 Blueprint에서 함수를 여러 개 묶어서 사용할 수 있도록 설정 
	 *  - Boss의 Attack Skill이 4개라면(블루프린트에서 4개를 어떻게 처리할 지 설정)
	 *	- 각각의 보스 패턴들은 C++로 만들어 BlueprintCallable 하거나, 블루프린트로 구현
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack(AActor* AttackTarget);

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
	void HitReact(EHitResponse HitResponse, AActor* DamageCursor);

	/*
	 * 공격을 방어하는 수단의 시작
	 *	- 방어 자세를 취하면서 Block한다
	 *	- HitReact에서 Block이 성공하면 방어 애니메이션을 취해주거나 반격한다
	 *	- Blcok이 실패한다면, 넉백이나 기절, 정지 등을 표현해주자
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartBlcok(EBlockingState BlockingState);

	/*
	 * 공격 방어를 마무리하는 단계
	 *	- 애니메이션을 가드에서 Idle 단계로 넘어갈 수 있도록 처리해준다
	 *  - 만약 필요하다면 가드가 끝난 직후 바로 스킬을 사용할 수 있도록 표현한다
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndBlcok(EBlockingState BlockingState);


	/*
	 * 공격 시작 전에 표현할 게 있다면 쓰는 함수
	 *	- 공격 시작전에 이펙트나 즉사 패턴기를 사용한다
	 *	- 적이 여러명이 한번에 공격하는 것을 방지하기 위해 토큰화를 처리한다
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartAttack(AActor* AttackTarget, int32 TokensNeeded);

	/*
	 * 공격 마무리에 표현할 게 있다면 쓰는 함수
	 *	- 보스의 패턴이 마무리되고 패턴을 회수할 때 사용한다
	 *	- 사용한 토큰을 다시 회수하는 역할도 수행할 수 있다
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndAttack(AActor* AttackTarget, int32 TokensNeeded);

public:
	UFUNCTION(BlueprintCallable)
	virtual void CallOnAttackEnd();

	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;

	UFUNCTION(BlueprintCallable)
	virtual void CallOnBlockEnd();
	
	UPROPERTY(BlueprintAssignable)
	FOnBlockEnded OnBlockEnded;


	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

public:
	void ReturnAttackToken(int32 Amount);

protected:
	UPROPERTY(EditAnywhere, Category = "S1|Patrol")
	TObjectPtr<AS1PatrolRoute> PatrolRoute;

public:
	UFUNCTION(BlueprintCallable)
	AS1PatrolRoute* GetPatrolRoute() const { return PatrolRoute; }

	UBehaviorTree* GetBehaviorTree() const { return BTAsset; }
	FIdealRange GetIdealRange() const { return IdealRange; }

	int32 GetCurrentAttackToken() const { return TokensUseInCurrentAttack; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
    TObjectPtr<US1HealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Attack")
	TObjectPtr<US1BotCombatSystemComponent> BotCombatSystemComponent;

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "S1|AI")
	TObjectPtr<UBehaviorTree> BTAsset;


protected:
	/** BT : 공격, 방어 탐지에 대한 변수 */
	UPROPERTY(EditAnywhere, Category = "S1|Interface")
	FIdealRange IdealRange;

	/** 이동속도 관련 변수 (GetMovementSpeed 에서 사용) */
	UPROPERTY(EditAnywhere, Category = "S1|Movement")
	FMovementState MovementState;

	/** 데미지를 적용할 수 있는 변수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "S1|Damage")
	TArray<FDamageInfo> DamageInfos;

protected:
	/** 현재 내가 소유한 공격 토큰이 몇 개 인지 */
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	int32 TokensUseInCurrentAttack;

	/** 공격 자체를 할 수 있는 지*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	bool bCanAttack;

	/** 공격 시작을 할 수 있는 지*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	bool bIsAttackStart;


protected:
	/** 방어(블록) 관련 변수 (Start, End, OnBlocked 에서 사용) */
	UPROPERTY(BlueprintReadWrite, Category = "S1|Block")
	EBlockingState BlockState;


protected:
	/** Nav Link 사용 시 점프 Z Velocity 값 */
	UPROPERTY(EditAnywhere, Category = "S1|AI|Jump")
	float ZVelocity = 250.f;
}; 
