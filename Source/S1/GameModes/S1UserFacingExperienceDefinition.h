// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "S1UserFacingExperienceDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class S1_API US1UserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/*
	 * member variables
	 */

	/** 맵 로딩과 관련된 ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	/** GameplayExperience 로딩과 관련된 ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "S1ExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
};
