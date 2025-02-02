
// Copyright (c) S1

#pragma once

#include "Equipment/S1GameplayAbility_FromEquipment.h"
#include "S1GameplayAbility_ProjectileWeapon.generated.h"

UCLASS()
class S1_API US1GameplayAbility_ProjectileWeapon : public US1GameplayAbility_FromEquipment
{
    GENERATED_BODY()

public:
    US1GameplayAbility_ProjectileWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable)
    void StartProjectileWeaponTargeting();

    /** TargetData가 준비되었을 때 호출한다 */
    UFUNCTION(BlueprintImplementableEvent)
    void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

}; 
