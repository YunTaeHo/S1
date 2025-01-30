
// Copyright (c) S1

#pragma once

#include "S1WeaponInstance.h"
#include "S1RangedWeaponInstance.generated.h"

UCLASS()
class S1_API US1RangedWeaponInstance : public US1WeaponInstance
{
    GENERATED_BODY()

public:
    US1RangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** 유효 사거리 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfing", meta = (ForceUnits = cm))
    float MaxDamageRange = 25000.f;

    /** 총탄 범위 (Sphere Trace Sweep) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfing", meta = (ForceUnits = cm))
    float BulletTraceWeaponRadius = 0.f;


}; 
