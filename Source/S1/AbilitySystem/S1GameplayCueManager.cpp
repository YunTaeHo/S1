﻿
// Copyright (c) S1

#include "S1GameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "System/S1AssetMaanger.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameplayCueManager)

US1GameplayCueManager* US1GameplayCueManager::Get()
{
    return Cast<US1GameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

US1GameplayCueManager::US1GameplayCueManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

void US1GameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
    TArray<FSoftObjectPath> CuePaths;
    UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
    if (RuntimeGameplayCueSet)
    {
        RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
    }

    // 새로 추가된 에셋의 경로를 CuePaths에 추가하고
    FAssetBundleData BundleData;
    BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);

    // PrimaryAssetId를 고정된 이름으로 설정하여 (const로 설정되어 있음)
    FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

    // 에셋 매니저에 추가 진행
    UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}


