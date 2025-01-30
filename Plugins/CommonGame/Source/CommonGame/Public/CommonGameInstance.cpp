
// Copyright (c) S1

#include "CommonGameInstance.h"
#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonGameInstance)

UCommonGameInstance::UCommonGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
    int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
    if (ReturnVal != INDEX_NONE)
    {
        // PrimaryPlayer는 처음만 캐싱
        if (!PrimaryPlayer.IsValid())
        {
            PrimaryPlayer = NewPlayer;
        }

        // GameUIManagerSubsystem를 통해 NotifyPlayerAdded() 호출로 GameLayout을 추가한다
        GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
    }

    return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
    if (PrimaryPlayer == ExistingPlayer)
    {
        PrimaryPlayer.Reset();
    }

    GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));
    return Super::RemoveLocalPlayer(ExistingPlayer);
}


