#pragma once

#include "S1CameraMode.generated.h"

/** 전방 선언 */
class US1CameraComponent;

/*
 *	[0,1]을 BlendFunction에 맞게 재매핑을 해주는 타입
 */
UENUM(BlueprintType)
enum class ES1CameraModeBlendFunction : uint8
{
	Linear,
	/*
	 *	EastIn/Out은 exponent 값에 의해 조절된다:
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT,
};

struct FS1CameraModeView
{
	FS1CameraModeView();

	void Blend(const FS1CameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/** Cmaera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class US1CameraMode : public UObject
{
	GENERATED_BODY()

public:
	US1CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

	void SetBlendWeight(float Weight);

	void UpdateCameraMode(float DeltaTime);

	FVector GetPivotLocation();
	FRotator GetPivotRotation();
	AActor* GetTargetActor() const;
	US1CameraComponent* GetS1CameraComponent() const;

	/** CameraMode에 의해 생성된 CameraModeView */
	FS1CameraModeView View;

	/** CameraMode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	// Pitch 값에 대해 Min/Max를 Clamp시킴
	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-85.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-85.9", ClampMax = "89.9"))
	float ViewPitchMax;


	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/*
	*	EaseIn/Out에 사용한 Exponent
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/** Blend function */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ES1CameraModeBlendFunction BlendFunction;

	/** 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/*
	 *	해당 CameraMode의 최종 Blend 값
	 *	앞서 BlendAplha의 선형 값을 매핑하여 최종 BlendWeight를 계산
	 */
	float BlendWeight;


protected:
	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;

};


/** Camera Blending을 담당하는 객체 */
UCLASS()
class US1CameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	US1CameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 *	member methods
	 */
	US1CameraMode* GetCameraModeInstance(TSubclassOf<US1CameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<US1CameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FS1CameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FS1CameraModeView& OutCameraModeView) const;

	/*
	 *	member variables
	 */

	/** 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<US1CameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	UPROPERTY()
	TArray<TObjectPtr<US1CameraMode>> CameraModeStack;
};