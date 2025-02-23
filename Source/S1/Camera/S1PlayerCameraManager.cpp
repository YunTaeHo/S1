
// Copyright (c) S1

#include "S1PlayerCameraManager.h"
#include "Player/S1PlayerController.h"
#include "Camera/S1CameraShake.h"
#include "GameFramework/Character.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerCameraManager)

AS1PlayerCameraManager::AS1PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    DefaultFOV = S1_CAMERA_DEFAULT_FOV;
    ViewPitchMin = S1_CAMERA_DEFAULT_PITCH_MIN;
    ViewPitchMax = S1_CAMERA_DEFAULT_PITCH_MAX;
}

void AS1PlayerCameraManager::UpdateView(float DeltaTime)
{
    UpdateShakes(DeltaTime);
    ApplyShake(DeltaTime);
}

void AS1PlayerCameraManager::UpdateShakes(float DeltaTime)
{
    ShakeSum = FRotator::ZeroRotator;


    if (LoopingShake)
    {
        FRotator ShakeValue;
        LoopingShake->UpdateShake(DeltaTime, ShakeValue);
        ShakeSum += ShakeValue;
    }

    if (ActiveShakes.Num() < 1)
    {
        return;
    }

    static TArray<int32> ExpiredShakeIndices;
    ExpiredShakeIndices.Reset();

    for (int32 i = 0; i < ActiveShakes.Num(); i++)
    {
        TObjectPtr<US1CameraShake> Shake = ActiveShakes[i];
        if (!Shake)
        {
            ExpiredShakeIndices.Add(i);
            continue;
        }

        FRotator ShakeValue;
        if (!Shake->UpdateShake(DeltaTime, ShakeValue))
        {
            ExpiredShakeIndices.Add(i);
        }
        ShakeSum += ShakeValue;
    }

    for (int32 i = ExpiredShakeIndices.Num() - 1; i >= 0; i--)
    {
        US1CameraShake* ExpiredShake = ActiveShakes[ExpiredShakeIndices[i]];
        if (ExpiredPool.Num() < 100)
        {
            ExpiredPool.Add(ExpiredShake);
        }
        ActiveShakes.RemoveAt(ExpiredShakeIndices[i]);
    }
}

void AS1PlayerCameraManager::ApplyShake(float DeltaTime)
{
    DeltaShake = ShakeSum - ShakeSumPrev;
    ShakeSumPrev = ShakeSum;

    if (ACharacter* Character = GetOwningCharacter())
    {
        Character->AddControllerPitchInput(DeltaShake.Pitch);
        Character->AddControllerYawInput(DeltaShake.Yaw);
    }


}

void AS1PlayerCameraManager::AddShake(float InDuration, UCurveVector* InCurve, FRotator InShakeMagnitude, bool bInLoop)
{
    US1CameraShake* Shake = ReclaimShakeFromExpiredPool();
    if (!Shake)
    {
        Shake = NewObject<US1CameraShake>(this);
    }

    if (bInLoop || InDuration <= 0)
    {
        if (LoopingShake && InCurve == LoopingShake->ControlShakeParams.Curve)
        {
            return;
        }

        ClearLoopingShake();
        LoopingShake = Shake;
        InDuration = -1.f;
    }
    else
    {
        ActiveShakes.Add(Shake);
    }

    Shake->Activate(InDuration, InCurve, InShakeMagnitude);
}

void AS1PlayerCameraManager::AddShake(FControlShakeParams Params, bool bInLoop)
{
    AddShake(Params.Duration, Params.Curve, Params.ShakeMagnitude, bInLoop);
}

void AS1PlayerCameraManager::ClearLoopingShake()
{
    if (LoopingShake)
    {
        LoopingShake->Clear();
        LoopingShake = nullptr;
    }
}

US1CameraShake* AS1PlayerCameraManager::ReclaimShakeFromExpiredPool()
{
    if (!ExpiredPool.IsEmpty())
    {
        return ExpiredPool.Pop();
    }
    return nullptr;
}

ACharacter* AS1PlayerCameraManager::GetOwningCharacter()
{
    if (!OwningCharacter)
    {
        if (AS1PlayerController* S1PC = Cast<AS1PlayerController>(GetOwner()))
        {
            OwningCharacter = Cast<ACharacter>(S1PC->GetCharacter());
        }
    }

    return OwningCharacter;
}

