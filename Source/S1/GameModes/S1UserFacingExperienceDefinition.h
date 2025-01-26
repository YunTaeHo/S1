// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "S1UserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionRequest;

/**
 * 
 */
UCLASS(BlueprintType)
class S1_API US1UserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/*
	 *	Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용해 HostSessionRequest 생성
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

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
