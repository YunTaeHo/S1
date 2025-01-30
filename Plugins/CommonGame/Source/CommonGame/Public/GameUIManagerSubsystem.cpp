
// Copyright (c) S1

#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"
#include "GameUIPolicy.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIManagerSubsystem)

UGameUIManagerSubsystem::UGameUIManagerSubsystem()
    : Super()
{

}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // CurrentPolicy가 설정되어 있지 않고, DefaultUIPolicyClass가 제대로 설정되어 있을 경우
    if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
    {
        // UIPolicyClass는 BP에셋이기 때문에 로딩해야 함
        TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();

        // UIPolicyClass를 통해 NewObject로 인스턴싱해서 CurrentPolicy에 설정
        SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
    }
}

void UGameUIManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
    SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    if (!Cast<UGameInstance>(Outer)->IsDedicatedServerInstance())
    {
        // 만약 어떠한 Subsystem도 UGameUIManagerSubsystem을 상속받지 않는다면, 해당 GameInstanceSubsystem은 활성화 안된다
        TArray<UClass*> ChildClass;
        GetDerivedClasses(GetClass(), ChildClass, false);

        return ChildClass.Num() == 0;
    }

    return false;
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
    if (CurrentPolicy != InPolicy)
    {
        CurrentPolicy = InPolicy;
    }
}


void UGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
    if (ensure(LocalPlayer) && CurrentPolicy)
    {
        CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
    }
}

void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
    if (LocalPlayer && CurrentPolicy)
    {
        CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
    }
}

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
    if (LocalPlayer && CurrentPolicy)
    {
        CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
    }
}



