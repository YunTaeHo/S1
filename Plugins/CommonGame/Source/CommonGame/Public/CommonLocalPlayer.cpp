
// Copyright (c) S1

#include "CommonLocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonLocalPlayer)

UCommonLocalPlayer::UCommonLocalPlayer()
    : Super(FObjectInitializer::Get())
{

}

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
    APlayerController* PC = GetPlayerController(GetWorld());
    APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

    if (PlayerState)
    {
        // PlayerState가 있으면 바로 호출
        Delegate.Execute(this, PlayerState);
    }

    // OnPlayerStateSet에 등록
    return OnPlayerStateSet.Add(Delegate);
}


