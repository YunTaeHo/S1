// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "S1GameModeBase.generated.h"

/** forward declkaretion */
class US1ExperienceDefinition;
class US1PawnData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnHandlerUpdateCompleted);

/**
 * 
 */
UCLASS()
class S1_API AS1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AS1GameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 *	AGmaeModeBase`s Interface
	 */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void InitGameState() final;

	/** SpawnEefaultPawnAtTransform*/
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;

	/*
	 *	member methods
	 */
	void HandleMatchAssignmentIfNoExpectingOne();
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const US1ExperienceDefinition* CurrentExperience);
	const US1PawnData* GetPawnDataForController(const AController* InController) const;
	
	/** PawnHandler의 업데이트가 모두 끝나면 호출시켜주는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnPawnHandlerUpdateCompleted OnPawnHandlerUpdateCompleted;

};
