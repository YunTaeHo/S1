
// Copyright (c) S1

#pragma once

#include "GameFramework/HUD.h"
#include "S1HUD.generated.h"

UCLASS()
class S1_API AS1HUD : public AHUD
{
    GENERATED_BODY()

public:
    AS1HUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * GameFrameworkComponentManager의 AddReceiver를 위한 메서드들
     */
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

}; 
