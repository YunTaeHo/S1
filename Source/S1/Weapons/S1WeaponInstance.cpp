
// Copyright (c) S1

#include "S1WeaponInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1WeaponInstance)

US1WeaponInstance::US1WeaponInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

TSubclassOf<UAnimInstance> US1WeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
    const FS1AnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
    return SetToQuery.SelectBestLayer(CosmeticTags);
}


