
// Copyright (c) S1

#include "CommonPlayerController.h"
#include "CommonLocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonPlayerController)

ACommonPlayerController::ACommonPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void ACommonPlayerController::ReceivedPlayer()
{
    Super::ReceivedPlayer();

    // PC가 LocalPlayer에 붙으면 이벤트를 활성화시켜준다
    if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Player))
    {
        LocalPlayer->OnplayerControllerSet.Broadcast(LocalPlayer, this);
    }
}


