// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "Interface/TeamInterface.h"
#include "Interface/BlockInterface.h"
#include "Misc/S1Container.h"
#include "S1Character.generated.h"

/** foward declarations */
class US1PawnHandler;
class US1CameraComponent;
class US1HealthComponent;
struct FDamageInfo;


UCLASS()
class S1_API AS1Character : public AModularCharacter, public IAbilitySystemInterface, public ICombatInterface, public ITeamInterface, public IBlockInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS1Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialized();

	/*
	 * ACharacter`s Interface
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

	/*
	 * ICombatInterface 
	 */
	virtual void DamageOnEvent(AActor* DamageCursor, FDamageInfo Info) override;
	virtual void TryToBlock() override;
	virtual void StartBlcok(EBlockingState BlockingState) override;
	virtual void Blocking(EBlockingState BlockingState) override;
	virtual void EndBlcok() override;
	virtual int32 GetTeamNumber() override;
	virtual bool IsDead() override;
	virtual void SetDead(bool bDead) override;
	virtual void Die() override;
	virtual void Attack(AActor* AttackTarget) override;
	virtual void HitReact(EHitResponse HitResponse, AActor* DamageCursor) override;


/** 공격 토큰 관련 함수*/
public:
	UFUNCTION(BlueprintCallable)
	bool ReserveAttackToken(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void ReturnAttackToken(int32 Amount);


/** 필요한 컴포넌트들 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1PawnHandler> PawnHandlerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1CameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1HealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1CombatSystemComponent> CombatSystemComponent;

};
