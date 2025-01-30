
// Copyright (c) S1

#pragma once

#include "GameplayCueManager.h"
#include "S1GameplayCueManager.generated.h"

UCLASS()
class S1_API US1GameplayCueManager : public UGameplayCueManager
{
    GENERATED_BODY()

public:
    static US1GameplayCueManager* Get();

    US1GameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void RefreshGameplayCuePrimaryAsset();

}; 
