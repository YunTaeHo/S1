
// Copyright (c) S1

#include "S1ExperienceManagerComponent.h"
#include "S1ExperienceDefinition.h"
#include "S1ExperienceActionSet.h"
#include "System/S1AssetMaanger.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystemSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ExperienceManagerComponent)

US1ExperienceManagerComponent::US1ExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1ExperienceManagerComponent::CallorRegister_OnExperienceLoaded(FOnS1ExperienceLoaded::FDelegate&& Delegate)
{
    if (IsExperienceLoaded())
    {
        Delegate.Execute(CurrentExperience);
    }
    else
    {
        // 복사 비용을 낮추기 위해 무브 시멘틱을 활용
        OnExperienceLoaded.Add(MoveTemp(Delegate));
    }
}

void US1ExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
    US1AssetMaanger& AssetManager = US1AssetMaanger::Get();

    TSubclassOf<US1ExperienceDefinition> AssetClass;
    {
        FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
        AssetClass = Cast<UClass>(AssetPath.TryLoad());
    }

    // CDO를 가져온다
    const US1ExperienceDefinition* Experience = GetDefault<US1ExperienceDefinition>(AssetClass);
    check(Experience != nullptr);
    check(CurrentExperience == nullptr);
    {
        // 가져온 CDO를 CurrentExperience에 넣어준다
        // - 해당 코드의 이유는 StartExperienceLoad에서 확인 가능
        CurrentExperience = Experience;
    }

    StartExperienceLoad();
}

void US1ExperienceManagerComponent::StartExperienceLoad()
{
    check(CurrentExperience);
    check(LoadState == ES1ExperienceLoadState::Unloaded);

    LoadState = ES1ExperienceLoadState::Loading;

    US1AssetMaanger& AssetManager = US1AssetMaanger::Get();

    TSet<FPrimaryAssetId> BundleAssetList;

    // GetPrimaryAssetId 코드를 살펴보면
    //  - CDO만 상위에 있는 클래스의 타입을 가져와서 PrimaryAssetId를 반환한다
    //  - 위에서 CDO로 CurrentExperience를 받아온 이유가 이것이다
    BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

    // ExperienceActionSet을 순회하며, BundleAssetList를 추가해주자
    for (const TObjectPtr<US1ExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet)
        {
            // AS_Shooter_XXX 가 추가된다
            // - BundleAssetList는 Bundle로 등록할 Root의 PrimaryDataAsset를 추가하는 과정이다
            BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
        }
    }

    // GameFeature를 사용하여, Expiernce에 바인딩된 GameFeautre Plugin을 로딩할 Bundle 이름을 추가한다
    TArray<FName> BundlesToLoad;
    {
        const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
        bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
        bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);

        // Client, Server로 쓸 Bundlee를 나눠준다
        if (bLoadClient)
        {
            BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
        }
        if (bLoadServer)
        {
            BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
        }

    }

    FStreamableDelegate OnAssetLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

    // BundleAssetList를 호출시켜준다+
    TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
        BundleAssetList.Array(),
        BundlesToLoad, 
        {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

    if (!Handle.IsValid() || Handle->HasLoadCompleted())
    {
        // 로딩이 완료되었다면 ExecuteDelegate를 통해 OnAssetLoadedDelegate를 호출
        FStreamableHandle::ExecuteDelegate(OnAssetLoadedDelegate);
    }
    else
    {
        Handle->BindCompleteDelegate(OnAssetLoadedDelegate);
        Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetLoadedDelegate]()
            {
                OnAssetLoadedDelegate.ExecuteIfBound();
            }));
    }

    // FrameNumber 확인용 변수(로딩 시간 체크)
    static int32 StartExperienceLoad_FrameNumber = GFrameNumber;

}

// StreamableDelegateDelayHelper에 의해 호출
void US1ExperienceManagerComponent::OnExperienceLoadComplete()
{
    // FrameNumber 확인용 변수(로딩 시간 체크)
    static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

    check(LoadState == ES1ExperienceLoadState::Loading);
    check(CurrentExperience);

    // 이전에 활성된 플러그인 클리어
    GameFeautrePluginURLs.Reset();

    auto CollectionGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
    {
        // FeaturePluginList를 순회하며, PluginRL을 ExperienceManagerComponent의 GameFeaturePluginURLs에 추가
        for (const FString& PluginName : FeaturePluginList)
        {
            FString PluginRL;
            if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginRL))
            {
                This->GameFeautrePluginURLs.AddUnique(PluginRL);
            }
        }
    };

    // 활성화할 GameFeature를 등록
    CollectionGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

    // GameFeaturePluginURLs에 등록된 Plugin을 로딩 및 활성화
    NumGameFeaturePluginsLoading = GameFeautrePluginURLs.Num();
    if (NumGameFeaturePluginsLoading)
    {
        LoadState = ES1ExperienceLoadState::LoadingGameFeatures;
        for (const FString& PluginURL : GameFeautrePluginURLs)
        {
            UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
        }
    }
    else
    {
        OnExperienceFullLoadComplete();
    }

}


void US1ExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
    NumGameFeaturePluginsLoading--;

    // GameFeature 로딩이 다 끝났어?
    if (NumGameFeaturePluginsLoading == 0)
    {
        // Loaded에서 마지막 마무리를 해주자
        OnExperienceFullLoadComplete();
    }
}



void US1ExperienceManagerComponent::OnExperienceFullLoadComplete()
{
    check(LoadState != ES1ExperienceLoadState::Loaded);

    // GameFeature Plugin의 로딩과 활성화 이후, GameFeature Action들을 활성화시키자:
    {
        LoadState = ES1ExperienceLoadState::ExecutingActions;

        // GameFeatureAction 활성화를 위한 Context 준비
        FGameFeatureActivatingContext Context;
        {
            // 월드의 핸들을 세팅해준다
            const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
            if (ExistingWorldContext)
            {
                Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
            }
        }

        auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
        {
            for (UGameFeatureAction* Action : ActionList)
            {
                // 명시적으로 UGameFeatureAction에 대해 Registering -> Loading -> Activating 순으로 호출
                if (Action)
                {
                    Action->OnGameFeatureRegistering();
                    Action->OnGameFeatureLoading();
                    Action->OnGameFeatureActivating(Context);
                }
            }
        };

        // 1. Experience의 Actions
        ActivateListOfActions(CurrentExperience->Actions);

        // 2. Experience의 ActionSets
        for (const TObjectPtr<US1ExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
        {
            ActivateListOfActions(ActionSet->Actions);
        }
    }

    LoadState = ES1ExperienceLoadState::Loaded;
    OnExperienceLoaded.Broadcast(CurrentExperience);
    OnExperienceLoaded.Clear();
}


const US1ExperienceDefinition* US1ExperienceManagerComponent::GetCurrentExperienceChecked() const
{
    check(LoadState == ES1ExperienceLoadState::Loaded);
    check(CurrentExperience != nullptr);
    return CurrentExperience;
}

