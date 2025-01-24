
// Copyright (c) S1

#pragma once

#include "S1MappableConfigPair.generated.h"

/** 전방 선언 */
class UPlayerMappableInputConfig;

USTRUCT()
struct FS1MappableConfigPair
{
    GENERATED_BODY()

public:
    FS1MappableConfigPair() = default;

    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UPlayerMappableInputConfig> Config;

    UPROPERTY(EditAnywhere)
    bool bShouldActivateAutomatically = true;


}; 
