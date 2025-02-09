#include "S1CameraMode.h"
#include "S1PlayerCameraManager.h"
#include "S1CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1CameraMode)

FS1CameraModeView::FS1CameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(S1_CAMERA_DEFAULT_FOV)
{
}

void FS1CameraModeView::Blend(const FS1CameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.f)
	{
		return;
	}
	else if (OtherWeight >= 1.f)
	{
		// Weight가 1.0이면 Other를 덮어쓰면 된다
		*this = Other;
		return;
	}

	// Location + OtherWeight OtherWeight * (Other.Location - Location)
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location과 같은 방식 Lerp(ControlRotation과 FieldOfView도 같음)
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}


US1CameraMode::US1CameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = S1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = S1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = S1_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.5f;
	BlendFunction = ES1CameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

FVector US1CameraMode::GetPivotLocation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			check(TargetCharacterCDO);

			const UCapsuleComponent* CapsuleComp = TargetCharacter->GetCapsuleComponent();
			check(CapsuleComp);

			const UCapsuleComponent* CapsuleCompCDO = TargetCharacterCDO->GetCapsuleComponent();
			check(CapsuleComp);

			// CDO와 현재를 비교해 보정해준다
			const float DefaultHalfHeight = CapsuleCompCDO->GetUnscaledCapsuleHalfHeight();
			const float ActualHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
			const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

			// 현재 Location에서 UpVector를 HeightAdjustment기준으로 보정시켜준다
			return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
		}

		// BaseEyeHeight를 고려하여, ViewLocation을 반환
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator US1CameraMode::GetPivotRotation()
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRotation을 가져와서 ControlRotation을 반환(공회전)
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

AActor* US1CameraMode::GetTargetActor() const
{
	const US1CameraComponent* S1CameraComponent = GetS1CameraComponent();
	return S1CameraComponent->GetTargetActor();
}

void US1CameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용하여, Pivot[Location|Rotation]을 계산하여, View를 업데이트
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

// Character의 Location과 ControllerRotation을 활용하여, View를 업데이트
void US1CameraMode::UpdateView(float DeltaTime)
{
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용하여, PivotLocation/Rotation을 반환함
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min/Max를 Clamp시킴
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FS1CameraModeView에 PivotLocation/Rotation 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용(ThriedView에서 적용)
	// - 특수한 로직 수행 시(아바타 모드) 바꾸게 된다
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

}

void US1CameraMode::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	// Since we're setting the blend weight directly, we need to calculate the blend alpha to account for the blend function.
	const float InvExponent = (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f;

	switch (BlendFunction)
	{
	case ES1CameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;

	case ES1CameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case ES1CameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case ES1CameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

void US1CameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha를 DeltaTime을 통해 계산
	if (BlendTime > 0.f)
	{
		// DeltaTime을 활용하여 진행도 누적
		// - 보가능ㄹ 몇 초 동안 해줄 것인가?
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.f;
	}

	// BlendWeight를 [0,1] BlendFunction에 맞게 재매핑
	const float Exponent = (BlendExponent > 0.f) ? BlendExponent : 1.f;
	switch (BlendFunction)
	{
	case ES1CameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ES1CameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ES1CameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ES1CameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending : Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}

}

US1CameraComponent* US1CameraMode::GetS1CameraComponent() const
{
	// CamerMode를 NewObject로 생성하면서 Outer를 US1CameraComponent로 넣어줘야지 캐스팅 가능
	return CastChecked<US1CameraComponent>(GetOuter());
}

US1CameraModeStack::US1CameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

PRAGMA_DISABLE_OPTIMIZATION
void US1CameraModeStack::PushCameraMode(TSubclassOf<US1CameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	US1CameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] 가장 최근에 이미 CameraMode가 Stacking되었으므로 바로 리턴
		return;
	}

	// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
	// ExistingStackContribution은 위에서 아래로 최종 BlendWeight 값을 찾기 위해 초기값으로 1.0 설정
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.f;

	/*
	*	BlendWeight		|	ExistingStackContribution	|	ExistingStackContribution (accumulated)
	*	0.1f			|	(1.0f) * 0.1f = 0.1f		|	(1.0f - 0.1f) = 0.9f
	*	0.3f			|	(0.9f) * 0.3f = 0.27f		|	(1.0f - 0.3f) * 0.9f = 0.63f
	*	0.6f			|	(0.63f) * 0.6f = 0.378f		|	(1.0f - 0.6f) * 0.63f = 0.252f
	*	1.0f			|	(0.252f) * 1.0f = 0.252f	|
	*					|	0.1f + 0.27 + 0.378f + 0.252f = 1.0f!
	*/

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			// 누적된 BlendWeight와 함께 BlendWeight를 곱해 루프를 빠져나간다
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// 값을 누적시켜서 원하는 CameraMode에서 가중치를 사용할 수있도록 설정
			ExistingStackContribution *= (1.f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	// 선택한 CameraMode를 Top으로 반영하기 위해, 제거하고 다시 Push를 수행해준다
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// CameraMode가 없었다면 Contribution을 0으로 세팅
		ExistingStackContribution = 0.f;
	}

	// BlendTime이 0보다 크다는 것은 Blend를 얼마 시간동안 진행함을 의미이다.
	// - 따라서 ExistingStackContribution를 적용한다
	// - Blend를 하지 않는다면, BlendWeight를 1.0에 넣어 새로 넣는 CameraMode만 적용할 것이다
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.f);

	CameraMode->SetBlendWeight(BlendWeight);

	// 0번 인덱스에 넣어준다 O(n)
	// 블렌드를 수행할 때  가중치에 따라 순회하면서 Remove를 하기 때문에 해당 방식으로 구현
	CameraModeStack.Insert(CameraMode, 0);

	// 반드시 Last는 BlendWeight가 1.f이 되어야함(0.f~1.f의 Contribution이 되어야 하기 때문)
	CameraModeStack.Last()->SetBlendWeight(1.f);

}

void US1CameraModeStack::EvaluateStack(float DeltaTime, FS1CameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]까지 순차적으로 Stack에 있는 CmaeraMode 업데이트
	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);
}

US1CameraMode* US1CameraModeStack::GetCameraModeInstance(TSubclassOf<US1CameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에서 먼저 새엇왿어있는 지 체크 후 변환
	for (US1CameraMode* CameraMode : CameraModeInstances)
	{
		// CameraMode는 UClass를 비교한다
		// - 즉 CameraMode는 클래스 타입에 하나만 생기게된다
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	US1CameraMode* NewCameraMode = NewObject<US1CameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void US1CameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack을 순회하며, CameraMode를 업데이트한다
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		US1CameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCaemraMode를 통해 보간을 처리해준다
		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 CameraMode의 BlendWeight가 하나라도  1.f에 도달했다면
		//	- 그 이후 CameraMode는 제거한다
		if (CameraMode->BlendWeight >= 1.f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void US1CameraModeStack::BlendStack(FS1CameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack은 Bottom -> Top 순서로 Blend를 진행한다
	const US1CameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->View;

	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}

PRAGMA_ENABLE_OPTIMIZATION
