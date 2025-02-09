
// Copyright (c) S1

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "S1PlayerCameraManager.generated.h"

/*
 *  Controller에 바인딩된 CameraManager
 */
#define S1_CAMERA_DEFAULT_FOV (80.f)
#define S1_CAMERA_DEFAULT_PITCH_MIN (-89.f)
#define S1_CAMERA_DEFAULT_PITCH_MAX (89.f)

UCLASS()
class S1_API AS1PlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    AS1PlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


}; 
