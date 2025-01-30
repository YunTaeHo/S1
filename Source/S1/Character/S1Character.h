// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "S1Character.generated.h"

class US1PawnHandler;
class US1CameraComponent;

UCLASS()
class S1_API AS1Character : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS1Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * ACharacter`s Interface
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1PawnHandler> PawnHandlerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<US1CameraComponent> CameraComponent;

};
