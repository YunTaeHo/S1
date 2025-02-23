
// Copyright (c) S1

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "GameplayTagContainer.h"
#include "S1PlayerCameraManager.generated.h"

/*
 *  Controller에 바인딩된 CameraManager
 */
#define S1_CAMERA_DEFAULT_FOV (80.f)
#define S1_CAMERA_DEFAULT_PITCH_MIN (-89.f)
#define S1_CAMERA_DEFAULT_PITCH_MAX (89.f)

/** foward delcarations */
class US1CameraShake;
class UCurveVector;
struct FControlShakeParams;

UCLASS()
class S1_API AS1PlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    AS1PlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void UpdateView(float DeltaTime);

protected:
	void UpdateShakes(float DeltaTime);
	void ApplyShake(float DeltaTime);

	FRotator ShakeSum;
	FRotator ShakeSumPrev;
	FRotator DeltaShake;

	float CurrentTime = 0.f;

public:
	void AddShake(float InDuration, UCurveVector* InCurve, FRotator InShakeMagnitude, bool bInLoop = false);

	UFUNCTION(BlueprintCallable)
	void AddShake(FControlShakeParams Params, bool bInLoop = false);

	UFUNCTION(BlueprintCallable)
	void ClearLoopingShake();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY()
	TArray<TObjectPtr<US1CameraShake>> ActiveShakes;

	UPROPERTY()
	TArray<TObjectPtr<US1CameraShake>> ExpiredPool;

	UPROPERTY()
	TObjectPtr<US1CameraShake> LoopingShake;

	US1CameraShake* ReclaimShakeFromExpiredPool();

	ACharacter* GetOwningCharacter();

}; 
