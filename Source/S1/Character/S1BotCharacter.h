
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Bot/BoInterface.h"
#include "S1BotCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);


/** foward declarations */
class US1PawnHandler;
class US1HealthComponent;
class UAbilitySystemComponent;
class UBehaviorTree;
class UWidgetComponent;
class US1GameplayAbility;
class UAnimMontage;


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
	virtual void BeginPlay() final;
	virtual void PossessedBy(AController* NewController) override;


/** Behavior Tree 관련 기본 상태(모든 Bot 들이 사용할 수 있도록 설정, 안쓸 거면 안써도 된다) */
public:
	UFUNCTION(BlueprintCallable)
	virtual void GetPatrolRoute() {}
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed() {}
	UFUNCTION(BlueprintCallable)
	virtual void DamageOnEvent(float Damage, TSubclassOf<UGameplayEffect> DamageEffect, AActor* DamageCursor);
	UFUNCTION(BlueprintCallable)
	virtual void JumpToDestination(FVector Location);
	UFUNCTION(BlueprintCallable)
	virtual void Die() {}

	/*
	 * 각자 공격을 Blueprint에서 함수를 여러 개 묶어서 사용할 수 있도록 설정 
	 *  - Boss의 Attack Skill이 4개라면(블루프린트에서 4개를 어떻게 처리할 지 설정)
	 *	- 각각의 보스 패턴들은 C++로 만들어 BlueprintCallable 하거나, 블루프린트로 구현
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack();

	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

public:
	UBehaviorTree* GetBehaviorTree() const { return BTAsset; }
	FIdealRange GetIdealRange() { return IdealRange; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
    TObjectPtr<US1HealthComponent> HealthComponent;

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Behavior")
	TObjectPtr<UBehaviorTree> BTAsset;



// 이 둘은 나중에 보스랑 비교해야해서 따로 뺄 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<UWidgetComponent> Widget;

	UPROPERTY(EditDefaultsOnly, Category = "S1|Widget")
	TSubclassOf<UUserWidget> HeathBarWidget;


protected:
	UPROPERTY(EditAnywhere, Category = "S1|Interface")
	FIdealRange IdealRange;


protected:
	/** Nav Link 사용 시 점프 Z Velocity 값 */
	UPROPERTY(EditAnywhere, Category = "AI|Jump")
	float ZVelocity = 250.f;
}; 
