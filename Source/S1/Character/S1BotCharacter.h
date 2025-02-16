
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Misc/S1Container.h"
#include "Interface/CombatInterface.h"
#include "Interface/TeamInterface.h"
#include "Interface/BlockInterface.h"
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
class UAnimMontage;
class USphereComponent;
class UBoxComponent;
class AS1BotController;
class UWidgetComponent;
class UClientUserWidget;
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
class S1_API AS1BotCharacter : public AModularCharacter, public IAbilitySystemInterface, public ICombatInterface, public ITeamInterface
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
	virtual int32 GetTeamNumber() override;
	virtual bool IsDead() override;
	virtual void SetDead(bool bDead) override;
	virtual void Die() override;
	UFUNCTION(BlueprintCallable)
	virtual void Attack(AActor* AttackTarget) override;
	virtual void HitReact(EHitResponse HitResponse, AActor* DamageCursor) override;


public:
	/*
	 * 공격 시작 전에 표현할 게 있다면 쓰는 함수
	 *	- 공격 시작전에 이펙트나 즉사 패턴기를 사용한다
	 *	- 적이 여러명이 한번에 공격하는 것을 방지하기 위해 토큰화를 처리한다
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StartAttack(AActor* AttackTarget, int32 TokensNeeded);

	/*
	 * 공격 마무리에 표현할 게 있다면 쓰는 함수
	 *	- 보스의 패턴이 마무리되고 패턴을 회수할 때 사용한다
	 *	- 사용한 토큰을 다시 회수하는 역할도 수행할 수 있다
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void EndAttack(AActor* AttackTarget, int32 TokensNeeded);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackMontage(AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitMontage(AActor* Target);
	

	/** 땅에 떨어졌을 때 자연스럽게 처리할 수 있는 함수(델리게이트에 묶어서 사용됨) */
	void OnLand(const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void UseLandDelegate();

/** Behavior Tree 관련 기본 상태(모든 Bot 들이 사용할 수 있도록 설정, 안쓸 거면 안써도 된다) */
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(EMovementState MoveState);

	UFUNCTION(BlueprintCallable)
	virtual void JumpToDestination(FVector Location);


public:
	UFUNCTION(BlueprintCallable)
	virtual void CallOnAttackEnd();

	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Attack")
	TObjectPtr<USphereComponent> Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Attack")
	TObjectPtr<UBoxComponent> Body;

	/** 보스는 위젯을 따로 빼서 사용해야함 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<UWidgetComponent> Widget;

	UPROPERTY(EditDefaultsOnly, Category = "S1|Widget")
	TSubclassOf<UClientUserWidget> HeathBarWidget;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "S1|AI")
	TObjectPtr<AS1BotController> BotController;

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "S1|AI")
	TObjectPtr<UBehaviorTree> BTAsset;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "S1|Attack")
	int32 TokensUseInCurrentAttack = 3;

	/** 공격 자체를 할 수 있는 지*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	bool bCanAttack;

	/** 공격 시작을 할 수 있는 지*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	bool bIsAttackStart;


	/** Hit 관련 반수 */
protected:
	/** Hit Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "S1|Hit")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/** Hit Montage Rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Hit")
	float HitMontagePlayRate = 1.f;

	/** Attack Montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Attack Montage Rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Attack")
	float AttackMontagePlayRate = 1.f;


protected:
	/** 방어(블록) 관련 변수 (Start, End, OnBlocked 에서 사용) */
	UPROPERTY(BlueprintReadWrite, Category = "S1|Block")
	EBlockingState BlockState;

protected:
	/** Nav Link 사용 시 점프 Z Velocity 값 */
	UPROPERTY(EditAnywhere, Category = "S1|AI|Jump")
	float ZVelocity = 250.f;
}; 
