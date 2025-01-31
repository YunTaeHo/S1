
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "S1ExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS()
class S1_API US1ExperienceActionSet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    US1ExperienceActionSet();

#if WITH_EDITORONLY_DATA
    virtual void UpdateAssetBundleData() override;
#endif

    /*
     * member variables
     */
    UPROPERTY(EditAnywhere, Category = "Actions to Perform")
    TArray<TObjectPtr<UGameFeatureAction>> Actions;

}; 
