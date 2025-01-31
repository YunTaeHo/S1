
// Copyright (c) S1

#include "S1HUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1HUD)

AS1HUD::AS1HUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1HUD::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    // S1HUD를 Receiver로 Actor를 추가한다
    UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AS1HUD::BeginPlay()
{
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
    Super::BeginPlay();
}

void AS1HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
    Super::EndPlay(EndPlayReason);
}


