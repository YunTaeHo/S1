
// Copyright (c) S1

#pragma once

#include "GameplayTagContainer.h"
#include "S1CosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FS1AnimBodyStyleSelectionEntry
{
    GENERATED_BODY()

    /** AnimLayer를 적용할 대상 SkeletalMesh */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> Mesh = nullptr;

    /** Cosmetic Tag */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
    FGameplayTagContainer RequiredTags;
};


USTRUCT(BlueprintType)
struct FS1AnimBodyStyleSelectionSet
{
    GENERATED_BODY()

    /** GameplayTag를 통해 (CosmeticTags), Mesh Rules에 따라 알맞은 BodyStyle를 반환한다 */
    USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

    /** AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation-Mesh간 Rules를 MeshRules라고 생각하면 됨 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FS1AnimBodyStyleSelectionEntry> MeshRules;

    /** 그냥 디폴트로 적용할 SkeletalMesh */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

    /*
     * Physics Asset은 하나로 통일함
     * 모든 Animation의 Physics 속성은 공유함 
     */
    UPROPERTY(EditAnywhere)
    TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;

};


