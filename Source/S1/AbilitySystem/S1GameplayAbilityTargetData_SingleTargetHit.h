
// Copyright (c) S1

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "S1GameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct FS1GameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
    GENERATED_BODY()

public:
    FS1GameplayAbilityTargetData_SingleTargetHit()
        : CartridgeID(-1)
    {}

    virtual UScriptStruct* GetScriptStruct() const
    {
        return FS1GameplayAbilityTargetData_SingleTargetHit::StaticStruct();
    }

    /** 탄약 ID (카트리지) */
    UPROPERTY()
    int32 CartridgeID;
}; 
