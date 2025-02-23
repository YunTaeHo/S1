
// Copyright (c) S1

#pragma once

#include "CommonPlayerController.h"
#include "S1PlayerController.generated.h"

/** foward declarations */
class AS1PlayerState;
class US1AbilitySystemComponent;
class AS1PlayerCameraManager;

UCLASS()
class S1_API AS1PlayerController : public ACommonPlayerController
{
    GENERATED_BODY()

public:
    AS1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * PlayerController`s Interface
     */
    virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

    /*
     * member methods
     */
    AS1PlayerState* GetS1PlayerState() const;
    US1AbilitySystemComponent* GetS1AbilitySystemComponent() const;
    AS1PlayerCameraManager* GetS1PlayerCameManager() const;

}; 
