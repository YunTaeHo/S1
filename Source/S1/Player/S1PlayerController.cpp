
// Copyright (c) S1

#include "S1PlayerController.h"
#include "Camera/S1PlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerController)

// 기본 생성자
AS1PlayerController::AS1PlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PlayerCameraManagerClass = AS1PlayerCameraManager::StaticClass();
}


