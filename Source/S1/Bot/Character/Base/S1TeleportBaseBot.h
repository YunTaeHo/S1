
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "Interface/TeleportInterface.h"
#include "S1TeleportBaseBot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportEnd);

UCLASS()
class S1_API AS1TeleportBaseBot : public AS1BotCharacter, public ITeleportInterface
{
    GENERATED_BODY()

public:
    AS1TeleportBaseBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable)
    virtual void Teleport(FVector Location) override;

    void Attack(AActor* Target) override;
   

    UFUNCTION(BlueprintImplementableEvent)
    void TeleporStart(FVector Location);

    UFUNCTION(BlueprintCallable)
    void TeleporEnd();

    UFUNCTION(BlueprintImplementableEvent)
    void DestroyedEffect();


    UPROPERTY(BlueprintAssignable)
    FOnTeleportEnd OnTeleportEnd;

}; 
