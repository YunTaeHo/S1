// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnumAI.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Passive,
    Attacking,
    Frozen,
    Investigating,
    Seeking,
    Dead,
};

UENUM(BlueprintType)
enum class EAISense : uint8
{
    None,
    Sight,
    Hearing,
    Damage,
};


/**
 * 
 */
UCLASS()
class S1_API UEnumAI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
