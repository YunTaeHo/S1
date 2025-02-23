
// Copyright (c) S1

#include "S1CameraShake.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1CameraShake)

US1CameraShake::US1CameraShake()
    : bIsActive(true)
    , TimeElapsed(0.f)
{
}

bool US1CameraShake::UpdateShake(float DeltaTime, FRotator& OutShake)
{
    //OutShake = FRotator::ZeroRotator;

    if (!bIsActive || !ControlShakeParams.Curve)
    {
        return false;
    }

    TimeElapsed += DeltaTime;

    float CurveTime = TimeElapsed;
    if (ControlShakeParams.Duration <= 0.f)
    {
        // Looping Shake
        float CurveStart;
        float CurveEnd;
        ControlShakeParams.Curve->GetTimeRange(CurveStart, CurveEnd);

        const float CurveLength = CurveEnd - CurveStart;
        TimeElapsed = CurveStart + FMath::Fmod(TimeElapsed, CurveLength);  // TimeElapsed 
        CurveTime = TimeElapsed;
    }
    else
    {
        CurveTime = UKismetMathLibrary::SafeDivide(TimeElapsed, ControlShakeParams.Duration);
        bIsActive = (CurveTime < 1.f);
    }

    const FVector CurveValue = bIsActive ? ControlShakeParams.Curve->GetVectorValue(CurveTime) : FVector::ZeroVector;

    OutShake = FRotator(
        ControlShakeParams.ShakeMagnitude.Pitch * CurveValue.X,
        ControlShakeParams.ShakeMagnitude.Yaw * CurveValue.Y,
        ControlShakeParams.ShakeMagnitude.Roll * CurveValue.Z  // Roll only affects weapon mesh.
    );

    if (!bIsActive)
    {
        Clear();
    }

    return bIsActive;
}

void US1CameraShake::Activate(float InDuration, UCurveVector* InCurve, FRotator InShakeMagnitude)
{
    bIsActive = true;
    TimeElapsed = 0.f;

    ControlShakeParams.Duration = InDuration;
    ControlShakeParams.Curve = InCurve;
    ControlShakeParams.ShakeMagnitude = InShakeMagnitude;
}

void US1CameraShake::Activate(FControlShakeParams InParams)
{
    Activate(InParams.Duration, InParams.Curve, InParams.ShakeMagnitude);
}

void US1CameraShake::Clear()
{
    bIsActive = false;
    TimeElapsed = 0.f;
    ControlShakeParams.Clear();
}