
// Copyright (c) S1

#pragma once

#include "Bot/Character/S1EquipmentDefaultBot.h"
#include "S1RangedDefaultBot_LineTrace.generated.h"

UCLASS()
class S1_API AS1RangedDefaultBot_LineTrace : public AS1EquipmentDefaultBot
{
    GENERATED_BODY()

public:
    AS1RangedDefaultBot_LineTrace(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void Attack(AActor* Target) override;
    virtual void EndAttack(AActor* AttackTarget, int32 TokensNeeded) override;
}; 
