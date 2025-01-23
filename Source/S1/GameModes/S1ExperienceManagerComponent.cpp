
// Copyright (c) S1

#include "S1ExperienceManagerComponent.h"
#include "S1ExperienceDefinition.h"
#include "System/S1AssetMaanger.h"
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

    // GameFeature를 사용하여, Expiernce에 바인딩된 GameFeautre Plugin을 로딩할 Bundle 이름을 추가한다
    TArray<FName> BundlesToLoad;
    {
        const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
        bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
        bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
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

    
    OnExperienceFullLoadComplete();
}

void US1ExperienceManagerComponent::OnExperienceFullLoadComplete()
{
    check(LoadState != ES1ExperienceLoadState::Loaded);

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


