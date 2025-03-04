﻿
// Copyright (c) S1

#pragma once

#include "Engine/GameInstance.h"
#include "CommonGameInstance.generated.h"

UCLASS(Abstract)
class COMMONGAME_API UCommonGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UCommonGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UGameInstance`s Interfaces
     */
    virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
    virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;

    /** 메인 로컬 플레이어를 캐싱 */
    TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
}; 
