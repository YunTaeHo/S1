#pragma once

#include "Engine/DataAsset.h"
#include "S1ExperienceDefinition.generated.h"

/** 전방 선언 */
class US1PawnData;

UCLASS(BlueprintType)
class US1ExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public: 
	US1ExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 *	member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<US1PawnData> DefaultPawnData;

	/*
	 *	마킹 및 기억용으로 남겨둔다
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

};
