
// Copyright (c) S1

#include "S1GameFeaturePolicy.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "AbilitySystemGlobals.h"
#include "GameFeatureAction_AddGameplayCuePath.h"
#include "AbilitySystem/S1GameplayCueManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameFeaturePolicy)

US1GameFeaturePolicy::US1GameFeaturePolicy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1GameFeaturePolicy::InitGameFeatureManager()
{
    // GameFeature_AddGameplayCuePaths를 등록
    Observers.Add(NewObject<US1GameFeature_AddGameplayCuePaths>());

    // Observers를 순회하며, GameFeaturesSubsystem에 Observer를 등록
    UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
    for (UObject* Observer : Observers)
    {
        Subsystem.AddObserver(Observer);
    }

    // 초기화 하는 과정에서 Observer가 등록된다
    Super::InitGameFeatureManager();
}

void US1GameFeaturePolicy::ShutdownGameFeatureManager()
{
    Super::ShutdownGameFeatureManager();

    UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
    for (UObject* Observer : Observers)
    {
        Subsystem.RemoveObserver(Observer);
    }
    Observers.Empty();
}

void US1GameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
    // PluginName을 활용하여, PluginRootPath를 계산
    const FString PluginRootPath = TEXT("/") + PluginName;

    // GameFeatureData의 Action을 순회
    // ex) ShooterCor의 GameFeatureData에 Action을 추가했다면, 그 추가된 Action이 대상이 된다
    for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
    {
        // 그중에 우리는 _AddGameplayCuepath를 찾아서 처리
        if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
        {
            const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;

            // GameplayCueManager를 가져와, GFA에 등록된 DirsToAdd를 추가하면서 GCM의 데이터가 업데이트 되도록 잔행
            if (US1GameplayCueManager* GCM = US1GameplayCueManager::Get())
            {
                // RuntimeCueSet을 가져옴
                UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
                const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;

                for (const FDirectoryPath& Directory : DirsToAdd)
                {
                    FString MutablePath = Directory.Path;

                    // PluginPackagePath를 한번 보정해 줌 -> 보정된 결과는 MutablePath로 들어옴
                    UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);

                    // GCM에서 RuntimeGameplayCueObjectLibrary의 Path를 추가
                    GCM->AddGameplayCueNotifyPath(MutablePath, /*bShouldRescanCueAssets=*/false);
                }

                // 초기화 시켜주고
                if (!DirsToAdd.IsEmpty())
                {
                    GCM->InitializeRuntimeObjectLibrary();
                }

                // Cue의 에셋이 Pre와 Post랑 비교해 갯수가 달라져있다면, 명시적으로 RefreshGameplayCuePrimaryAsset를 호출
                const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
                if (PreInitializeNumCues != PostInitializeNumCues)
                {
                    // 추가된 GCN을 AsssetManager에 등록해준다
                    GCM->RefreshGameplayCuePrimaryAsset();
                }
            }
        }
    }
}

void US1GameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
    const FString PluginRootPath = TEXT("/") + PluginName;
    for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
    {
        if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
        {
            const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;
            if (US1GameplayCueManager* GCM = US1GameplayCueManager::Get())
            {
                int32 NumRemoved = 0;
                for (const FDirectoryPath& Directory : DirsToAdd)
                {
                    FString MutablePath = Directory.Path;
                    UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
                    NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, /*bShouldRescanCueAssets=*/false);
                }

                ensure(NumRemoved == DirsToAdd.Num());

                if (NumRemoved > 0)
                {
                    GCM->InitializeRuntimeObjectLibrary();
                }
            }
        }
    }
}
