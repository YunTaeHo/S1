
// Copyright (c) S1

#pragma once

#include "GameFramework/PlayerState.h"
#include "S1BotPlayerState.generated.h"

/** foward declarations */
class US1PawnData;
class US1AbilitySystemComponent;

UCLASS()
class S1_API AS1BotPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AS1BotPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void PostInitializeComponents() final;

    /*
    *  member methods
    */
    US1AbilitySystemComponent* GetS1AbilitySystemComponent() const { return AbilitySystemComponent; }

    UPROPERTY(VisibleAnywhere, Category = "S1|PlayerState")
    TObjectPtr<US1AbilitySystemComponent> AbilitySystemComponent;

}; 
