﻿
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCuePath.generated.h"

UCLASS()
class S1_API UGameFeatureAction_AddGameplayCuePath : public UGameFeatureAction
{
    GENERATED_BODY()

public:
    UGameFeatureAction_AddGameplayCuePath();

    /** GameplayCueNotify 에셋 경로 추가 */
    UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
    TArray<FDirectoryPath> DirectoryPathsToAdd;


}; 
