
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "Equipment/S1GameplayAbility_FromEquipment.h"
#include "S1GameplayAbility_RangedWeapon.generated.h"

/** forward declaractions */
class US1RangedWeaponInstance;

/** 어디 대상으로 Target을 설정할지에 대한 옵션(다양한 옵션 기능 추가 가능) */
UENUM(BlueprintType)
enum class ES1AbilityTargetingSource : uint8
{
    /** Camera 기준으로 진행 */
    CameraTowardsFocus,
};

UCLASS()
class S1_API US1GameplayAbility_RangedWeapon : public US1GameplayAbility_FromEquipment
{
    GENERATED_BODY()

public:
    struct FRangedWeaponFiringInput
    {
        FVector StartTrace;
        FVector EndAim;
        FVector AimDir;
        US1RangedWeaponInstance* WeaponData = nullptr;
        bool bCanPlayBulletFX = false;

        FRangedWeaponFiringInput()
            : StartTrace(ForceInitToZero)
            , EndAim(ForceInitToZero)
            , AimDir(ForceInitToZero)
        {}
    };

    US1GameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable)
    void StartRangedWeaponTargeting();

    /** TargetData가 준비되었을 때 호출한다 */
    UFUNCTION(BlueprintImplementableEvent)
    void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

    US1RangedWeaponInstance* GetWeaponInstance();
    FVector GetWeaponTargetingSourceLocation() const;
    FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
        float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits);
    void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
    void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
    ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
    FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
    FTransform GetTargetingTransform(APawn* SourcePawn, ES1AbilityTargetingSource Source);
    void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

    void PerformLocalTargeting(TArray<FHitResult>& OutHits);

}; 
