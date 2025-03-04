﻿
// Copyright (c) S1

#pragma once

#include "Containers/Map.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

class FDelegateHandle;
class UGameInstance;
struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;
struct FWorldContext;

UCLASS(Abstract)
class S1_API UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
    GENERATED_BODY()

public:
    UGameFeatureAction_WorldActionBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UGameFeatureAction`s Interface
     */
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

    /*
     * Interface
     */
    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );

}; 
