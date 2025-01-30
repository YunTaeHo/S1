
// Copyright (c) S1

#include "S1CosmeticAnimationTypes.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1CosmeticAnimationTypes)

USkeletalMesh* FS1AnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
    // MeshRule을 순회하며, CosmeticTags 요구 조건에 맞는 MeshRule을 찾아 SkeletalMesh를 반환한다
    for (const FS1AnimBodyStyleSelectionEntry& Rule : MeshRules)
    {
        if ((Rule.Mesh) && (CosmeticTags.HasAll(Rule.RequiredTags)))
        {
            return Rule.Mesh;
        }
    }
    return DefaultMesh;
}

TSubclassOf<UAnimInstance> FS1AnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
    for (const FS1AnimLayerSelectionEntry& Rule : LayerRules)
    {
        if ((Rule.Layer) && (CosmeticTags.HasAll(Rule.RequiredTags)))
        {
            return Rule.Layer;
        }
    }
    return DefaultLayer;
}
