
// Copyright (c) S1

#pragma once

#include "Camera/CameraComponent.h"
#include "S1CameraComponent.generated.h"

/** 전방 선언 */
class US1CameraModeStack;
class US1CameraMode;
/** 템플릿 전방 선언 */
template<class TClass> class TSubclassOf;


/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<US1CameraMode>, FS1CameraModeDelegate);


UCLASS()
class S1_API US1CameraComponent : public UCameraComponent
{
    GENERATED_BODY()

public:
    US1CameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    static US1CameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<US1CameraComponent>() : nullptr); }

    /*
     *  CameraComponent`s Interfaces
     */
    virtual void OnRegister() final;
    virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;



    /*
     *  member methods
     */
    void UpdateCameraModes();
    AActor* GetTargetActor() const { return GetOwner(); }

    /*
     *  member variables
     */
    /** 카메라의 blending 기능을 지원하는 Stack */
    UPROPERTY()
    TObjectPtr<US1CameraModeStack> CameraModeStack;

    /** 현재 CameraMode를 가져오는 Delegate */
    FS1CameraModeDelegate DetermineCameraModeDelegate;

}; 
