
// Copyright (c) S1

#include "S1PlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerCameraManager)

AS1PlayerCameraManager::AS1PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    DefaultFOV = S1_CAMERA_DEFAULT_FOV;
    ViewPitchMin = S1_CAMERA_DEFAULT_PITCH_MIN;
    ViewPitchMax = S1_CAMERA_DEFAULT_PITCH_MAX;
}


