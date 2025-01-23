
// Copyright (c) S1

#include "S1GameInstance.h"
#include "S1GameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameInstance)

US1GameInstance::US1GameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1GameInstance::Init()
{
    Super::Init();

    // GameplayTag를 등록
    UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
    if (ensure(ComponentManager))
    {
        const FS1GameplayTags& GameplayTags = FS1GameplayTags::Get();

        // 첫 번째 인자를 기준으로 추가하게 된다
        ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
        ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
        ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
        ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);

    }
}

void US1GameInstance::Shutdown()
{
    Super::Shutdown();
}


