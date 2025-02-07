
// Copyright (c) S1

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "S1Container.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
    Idle,
    Walking,
    Jogging,
    Sprinting,
};

UCLASS()
class S1_API US1Container : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
}; 
