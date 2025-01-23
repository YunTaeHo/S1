
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "S1GameState.generated.h"

class US1ExperienceManagerComponent;

UCLASS()
class S1_API AS1GameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AS1GameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    UPROPERTY()
    TObjectPtr<US1ExperienceManagerComponent> ExperienceManagerComponent;

}; 
