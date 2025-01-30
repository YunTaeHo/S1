
// Copyright (c) S1

#pragma once

#include "Engine/LocalPlayer.h"
#include "Delegates/Delegate.h"
#include "CommonLocalPlayer.generated.h"


UCLASS()
class COMMONGAME_API UCommonLocalPlayer : public ULocalPlayer
{
    GENERATED_BODY()

public:
    UCommonLocalPlayer();

    /** PC가 Local Player에 할당되었을 경우 실행할 Delegate */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
    FPlayerControllerSetDelegate OnplayerControllerSet;
}; 
