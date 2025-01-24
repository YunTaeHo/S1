
// Copyright (c) S1

#include "S1PlayerComponent.h"
#include "S1LogChannel.h"
#include "S1PawnHandler.h"
#include "S1GameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/S1PlayerState.h"
#include "Camera/S1CameraComponent.h"
#include "Character/S1PawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerComponent)

const FName US1PlayerComponent::NAME_ActorFeatureName("PlayerComponent");

US1PlayerComponent::US1PlayerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<US1CameraMode> US1PlayerComponent::DetermineCameraMode() const
{
    const APawn* Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return nullptr;
    }

    if (US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
    {
        if (const US1PawnData* PawnData = PawnHandler->GetPawnData<US1PawnData>())
        {
            return PawnData->DefaultCameraMode;
        }
    }

    return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION



void US1PlayerComponent::OnRegister()
{
    Super::OnRegister();

    // 올바른 Actor에 등록되었는지 확인:
    {
        if (!GetPawn<APawn>())
        {
            UE_LOG(LogS1, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
        }
    }

    RegisterInitStateFeature();
}

void US1PlayerComponent::BeginPlay()
{
    Super::BeginPlay();

    // PawnHandler에 대해서 (PawnHandler Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
    BindOnActorInitStateChanged(US1PawnHandler::NAME_ActorFeatureName, FGameplayTag(), false);

    // InitState_Spawned로 초기화
    ensure(TryToChangeInitState(FS1GameplayTags::Get().InitState_Spawned));

    // ForceUpdate 진행
    CheckDefaultInitialization();
}

void US1PlayerComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    UnregisterInitStateFeature();
    Super::EndPlay(EndPlayReason);
}

void US1PlayerComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
    const FS1GameplayTags InitTags = FS1GameplayTags::Get();
    if (Params.FeatureName == US1PawnHandler::NAME_ActorFeatureName)
    {
        // US1PawnHandler가 DataInitialized라면?
        if (Params.FeatureState == InitTags.InitState_DataInitialized)
        {
            // 나도 DataInitialized로 전환하고 싶다
            CheckDefaultInitialization();
        }
    }
}

bool US1PlayerComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
    check(Manager);

    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();
    APawn* Pawn = GetPawn<APawn>();
    AS1PlayerState* S1PS = GetPlayerState<AS1PlayerState>();

    // Spawned 초기화
    if(!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
    {
        if (Pawn)
        {
            return true;
        }
    }

    // Spawned -> DataAvailable
    if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
    {
        // PawnData가 설정되어있지 않다면 InitState를 작동시키지 않는다
        if (!S1PS)
        {
            return false;
        }

        return true;
    }

    // DataAvailable -> DataInitialized
    if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
    {
        // US1PawnHandler가 DataInitialized될 때까지 기다린 (== 모든 Feature Component가 DataAvailable 인 상태
        return S1PS && Manager->HasFeatureReachedInitState(Pawn, US1PawnHandler::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
    }

    // DataInitialized -> GameplayReady
    if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
    {
        return true;
    }

    return false;
}

void US1PlayerComponent::CheckDefaultInitialization()
{
    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();

    // PawnHandler와 같은 코드
    static const TArray<FGameplayTag> StateChain =
    {
        InitTags.InitState_Spawned,
        InitTags.InitState_DataAvailable,
        InitTags.InitState_DataInitialized,
        InitTags.InitState_GameplayReady
    };

    ContinueInitStateChain(StateChain);
}

void US1PlayerComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
    const FS1GameplayTags InitTags = FS1GameplayTags::Get();

    // DataAvailable -> DataInitialized
    if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
    {
        APawn* Pawn = GetPawn<APawn>();
        AS1PlayerState* S1PS = GetPlayerState<AS1PlayerState>();
        if (!ensure(Pawn && S1PS))
        {
            return;
        }

        // @TODO Input과 Camera에 대한 핸들링

        const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
        const US1PawnData* PawnData = nullptr;
        if (US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
        {
            PawnData = PawnHandler->GetPawnData<US1PawnData>();
        }

        if (bIsLocallyControlled && PawnData)
        {
            // 현재 Character에 Attach된 CameraComponent를 찾음
            if (US1CameraComponent* CameraComponent = US1CameraComponent::FindCameraComponent(Pawn))
            {
                CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
            }
        }
    }
}

