
// Copyright (c) S1

#pragma once

#include "Equipment/S1EquipmentInstance.h"
#include "Cosmetics/S1CosmeticAnimationTypes.h"
#include "S1WeaponInstance.generated.h"

struct FDamageInfo;


UCLASS()
class S1_API US1WeaponInstance : public US1EquipmentInstance
{
    GENERATED_BODY()

public:
    US1WeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Weapon에 적용할 AnimLayer를 선택하여 반환 */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Animation")
    TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

    /*
     *  애니메이션 바꾸고 싶은 게 있을 때마다 추가해주자
     */

    /** Weapon에 Equip/Unequip에 대한 Animation Set 정보를 들고 있다 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
    FS1AnimLayerSelectionSet EquippedAnimSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
    FS1AnimLayerSelectionSet UnequippedAnimSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
    TArray<FDamageInfo> DamageInfos;
}; 
