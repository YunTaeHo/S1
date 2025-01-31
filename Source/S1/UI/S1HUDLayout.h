// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S1ActivatableWidget.h"
#include "S1HUDLayout.generated.h"

/**
 * PrimaryGameLayout에 Layer를 연동할 HUD Layout (CommonActivatableWidget)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta = (DisplayName = "S1 HUD Layout", Category = "S1|HUD"))
class S1_API US1HUDLayout : public US1ActivatableWidget
{
	GENERATED_BODY()
public:
	US1HUDLayout(const FObjectInitializer& ObjectInitializer);
};
