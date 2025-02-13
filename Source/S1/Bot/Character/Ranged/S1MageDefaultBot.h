
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "Bot/Character/Base/S1TeleportBaseBot.h"
#include "S1MageDefaultBot.generated.h"

UCLASS()
class S1_API AS1MageDefaultBot : public AS1TeleportBaseBot
{
    GENERATED_BODY()

public:
    AS1MageDefaultBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void Teleport(FVector Location) override;
}; 
