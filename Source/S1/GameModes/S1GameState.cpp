
// Copyright (c) S1

#include "S1GameState.h"
#include "S1ExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameState)


AS1GameState::AS1GameState(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ExperienceManagerComponent = CreateDefaultSubobject<US1ExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
