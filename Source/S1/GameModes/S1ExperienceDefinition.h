#pragma once

#include "Engine/DataAsset.h"
#include "S1ExperienceDefinition.generated.h"

/** 전방 선언 */
class US1PawnData;
class US1ExperienceActionSet;
class UGameFeatureAction;

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
	 * 마킹 및 기억용으로 남겨둔다(GameFeature 로딩용)
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/** ExperienceActionSet은 UGameFeatureAcion의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<US1ExperienceActionSet>> ActionSet;

	/** 일반적인 GameFeatureAction 추가*/
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Gameplay")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

};
