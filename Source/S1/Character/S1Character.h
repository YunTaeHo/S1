// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "Misc/S1Container.h"
#include "S1Character.generated.h"

/** foward declarations */
class US1PawnHandler;
class US1CameraComponent;
class US1HealthComponent;
struct FDamageInfo;


UCLASS()
class S1_API AS1Character : public AModularCharacter, public IAbilitySystemInterface, public ICombatInterface
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

/** 데미지 입력 관련 함수 */
public:
	virtual void DamageOnEvent(AActor* DamageCursor, FDamageInfo Info) override;

/** 공격 토근 관련 함수*/
public:
	UFUNCTION(BlueprintCallable)
	bool ReserveAttackToken(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void ReturnAttackToken(int32 Amount);


private:
	void HitReact(EHitResponse Respose, AActor* Target);
	void Die();

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
