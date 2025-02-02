
// Copyright (c) S1

#pragma once

#include "ModularAIController.h"
#include "S1PlayerBotController.generated.h"

UCLASS()
class S1_API AS1PlayerBotController : public AModularAIController
{
    GENERATED_BODY()

public:
    AS1PlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void OnUnPossess() override;

protected:
    virtual void InitPlayerState() override;
    virtual void CleanupPlayerState() override;

private:
    UPROPERTY()
    TObjectPtr<APlayerState> LastSeenPlayerState;

}; 
