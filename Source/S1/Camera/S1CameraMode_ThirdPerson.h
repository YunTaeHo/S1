
// Copyright (c) S1

#pragma once

#include "S1CameraMode.h"
#include "S1CameraMode_ThirdPerson.generated.h"

/** 전방 선언*/
class UCurveVector;

UCLASS(Abstract, Blueprintable)
class S1_API US1CameraMode_ThirdPerson : public US1CameraMode
{
    GENERATED_BODY()

public:
    US1CameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     *  US1CameraMode`s Interface
     */
    virtual void UpdateView(float DeltaTime) final;

    /*
     * member variables
     */
    UPROPERTY(EditDefaultsOnly, Category = "Third Person")
    TObjectPtr<const UCurveVector> TargetOffsetCurve;


}; 
