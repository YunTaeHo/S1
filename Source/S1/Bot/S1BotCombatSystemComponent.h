
// Copyright (c) S1

#pragma once

#include "Combat/S1CombatSystemComponent.h"
#include "S1BotCombatSystemComponent.generated.h"

UCLASS()
class S1_API US1BotCombatSystemComponent : public US1CombatSystemComponent
{
    GENERATED_BODY()

public:
    US1BotCombatSystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

}; 
