
// Copyright (c) S1

#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "S1BotCharacter.generated.h"

/** foward declarations */
class US1PawnHandler;
class US1HealthComponent;
class UAbilitySystemComponent;

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

	/*
	 * IAbilitySystemInterface
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const final;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
    TObjectPtr<US1HealthComponent> HealthComponent;


	

}; 
