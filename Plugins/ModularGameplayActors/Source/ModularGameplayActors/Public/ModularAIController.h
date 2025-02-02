
// Copyright (c) S1

#pragma once

#include "AIController.h"
#include "ModularAIController.generated.h"

class UObject;

UCLASS(Blueprintable)
class MODULARGAMEPLAYACTORS_API AModularAIController : public AAIController
{
    GENERATED_BODY()

public:

    /** AAIController`s Interfaces */
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

}; 
