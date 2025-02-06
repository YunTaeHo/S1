
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Bot/BoInterface.h"
#include "S1BotCharacter.generated.h"

/** foward declarations */
class US1PawnHandler;
class US1HealthComponent;
class UAbilitySystemComponent;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class S1_API AS1BotCharacter : public AModularCharacter, public IAbilitySystemInterface, public IBoInterface
{
    GENERATED_BODY()

public:
    AS1BotCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * ACharacter`s Interface
	 */
	virtual void BeginPlay() final;
	virtual void PossessedBy(AController* NewController) override;


public:
	/** IBoInterface`s Interface */
	virtual void GetPatrolRoute() override;
	virtual void SetMovementSpeed() override;
	virtual FIdealRange GetIdealRange() override;
	virtual void EquipWeapon() override;
	virtual void UnequipWeapon() override;
	virtual void Attack() override;
	virtual void DamageOnEvent(float Damage, TSubclassOf<UGameplayEffect> DamageEffect, AActor* DamageCursor) override;
	virtual void JumpToDestination(FVector Location) override;
	virtual void Die() override;

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

public:
	UBehaviorTree* GetBehaviorTree() const { return BTAsset; }

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
    TObjectPtr<US1HealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<UWidgetComponent> Widget;

	/** Behavior Tree */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Behavior")
	TObjectPtr<UBehaviorTree> BTAsset;

	UPROPERTY(EditAnywhere, Category = "S1|Interface")
	FIdealRange IdealRange;

	/** Nav Link 사용 시 점프 Z Velocity 값 */
	UPROPERTY(EditAnywhere, Category = "AI|Jump")
	float ZVelocity = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "S1|Widget")
	TSubclassOf<UUserWidget> HeathBarWidget;
}; 
