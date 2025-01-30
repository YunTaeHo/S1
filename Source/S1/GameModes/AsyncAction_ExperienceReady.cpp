
// Copyright (c) S1

#include "AsyncAction_ExperienceReady.h"
#include "S1ExperienceDefinition.h"
#include "S1ExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_ExperienceReady)

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
    UAsyncAction_ExperienceReady* Action = nullptr;
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        Action = NewObject<UAsyncAction_ExperienceReady>();
        Action->WorldPtr = World;
        Action->RegisterWithGameInstance(World);
    }

    return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
    if (UWorld* World = WorldPtr.Get())
    {
        // GameState가 이미 World에 준비되어 있으면 Step1을 스킵하고 바로 Step2를 진행한다
        if (AGameStateBase* GameState = World->GetGameState())
        {
            Step2_ListenToExperienceLoading(GameState);
        }
        else
        {
            // 아직 준비되어 있지 않다면, World->GameStateSetEvent에 Step1_HandleGameStateSet를 바인딩한다
            World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
        }
    }
    else
    {
        // World가 Valid 하지 않으므로 제거
        // GameInstance에서 래패런스 대상으로 제거
        SetReadyToDestroy();
    }
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
    if (UWorld* World = WorldPtr.Get())
    {
        World->GameStateSetEvent.RemoveAll(this);
    }

    Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
    check(GameState);

    US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
    check(ExperienceManagerComponent);

    // 만약 이미 Experience가 준비되었다면, Step3 스킵하고 Step4로 이동
    if (ExperienceManagerComponent->IsExperienceLoaded())
    {
        UWorld* World = GameState->GetWorld();
        check(World);

        // 이미 Experience가 준뵈되었다고 해도, 혹시 모르니 다음 Tick에서 사용할 수 있도록 바인딩
        World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
    }
    else
    {
        // 준비가 안되어있다면, OnExperienceLoaded에 바인딩시칸다 
        ExperienceManagerComponent->CallorRegister_OnExperienceLoaded(FOnS1ExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Step3_HandleExperienceLoaded));
    }
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const US1ExperienceDefinition* CurrentExperience)
{
    // @TODO 나중에 필요한 거 추가

    Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
    // 바인딩된 BP 혹은 UFUNCTION 호출
    OnReady.Broadcast();
    SetReadyToDestroy();
}


