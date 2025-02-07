
// Copyright (c) S1

#pragma once

#include "GameFramework/Actor.h"
#include "S1PatrolRoute.generated.h"

/** foward declarations */
class USplineComponent;

UCLASS()
class S1_API AS1PatrolRoute : public AActor
{
    GENERATED_BODY()

public:
    AS1PatrolRoute(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	/** 현재 PatrolIndex의 Position을 반환 */
	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointAsWorldPosition();

	/** PatrolIndex를 증가시켜준다 */
	UFUNCTION(BlueprintCallable)
	void IncrementPartolRoute();


protected:
	UPROPERTY(VisibleAnywhere, Category = "S1|Character")
	USplineComponent* RatrolRoute;

private:

	int32 PatrolIndex;
	int32 Direction;


}; 
