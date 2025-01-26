
// Copyright (c) S1

#include "GameFeatureAction_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Input/S1MappableConfigPair.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Character/S1PlayerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputConfig)

UGameFeatureAction_AddInputConfig::UGameFeatureAction_AddInputConfig(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    FPerContextData& ActivateData = ContextData.FindOrAdd(Context);
    if (!ensure(ActivateData.ExtensionRequestHandles.IsEmpty()) ||
        !ensure(ActivateData.PawnAddedTo.IsEmpty()))
    {
        Reset(ActivateData);
    }

    Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    Super::OnGameFeatureDeactivating(Context);
    FPerContextData* ActivateData = ContextData.Find(Context);
    if (ensure(ActivateData))
    {
        Reset(*ActivateData);
    }
}

void UGameFeatureAction_AddInputConfig::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
    UWorld* World = WorldContext.World();
    UGameInstance* GameInstance = WorldContext.OwningGameInstance;
    FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
    if (GameInstance && World && World->IsGameWorld())
    {
        // GFCM을 이용하여, ExtensionHandler를 추가해 등록 진행
        // - HandlePawnExtension 콜백함수로 연결
        if (UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
        {
            UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
                UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

            // 등록된 콜백 함수의 핸들을 ActiveData의 ExtensionRequestHandles에 등록
            TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = GFCM->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
            ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
        }
    }
}

void UGameFeatureAction_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
    APawn* AsPawn = CastChecked<APawn>(Actor);
    FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

    if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == US1PlayerComponent::NAME_BindInputsNow)
    {
        AddInputConfig(AsPawn, ActiveData);
    }
    else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == US1PlayerComponent::NAME_BindInputsNow)
    {
        RemoveInputConfig(AsPawn, ActiveData);
    }
}

void UGameFeatureAction_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
    APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
    if (ULocalPlayer* LP = PC ? PC->GetLocalPlayer() : nullptr)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            FModifyContextOptions Options = {};
            Options.bIgnoreAllPressedKeysUntilRelease = false;

            // InputConfig를 순회하며, Config를 직접 추가
            for (const FS1MappableConfigPair& Pair : InputConfigs)
            {
                Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
            }

            ActiveData.PawnAddedTo.AddUnique(Pawn);
        }
    }
}

void UGameFeatureAction_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
    APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
    if (ULocalPlayer* LP = PC ? PC->GetLocalPlayer() : nullptr)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            // InputConfig를 순회하며, Config를 제거 진행
            for (const FS1MappableConfigPair& Pair : InputConfigs)
            {
                Subsystem->RemovePlayerMappableConfig(Pair.Config.LoadSynchronous());
            }

            ActiveData.PawnAddedTo.Remove(Pawn);
        }
    }
}

void UGameFeatureAction_AddInputConfig::Reset(FPerContextData& ActiveData)
{
    // ExtensionRequestHandles을 초기화
    ActiveData.ExtensionRequestHandles.Empty();

    // PawnAddedTo에 대해서, 하나씩 Stack 방식으로 위에서 아래로 직접 InputConfig를 제거 진행
    while (!ActiveData.PawnAddedTo.IsEmpty())
    {
        TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnAddedTo.Top();
        if (PawnPtr.IsValid())
        {
            RemoveInputConfig(PawnPtr.Get(), ActiveData);
        }
        else
        {
            // WeakObjectPtr로 PawnsAddedTo를 관리하고 있기 때문에, GC되었다면, nullptr될 수 있음
            ActiveData.PawnAddedTo.Pop();
        }
    }
}


