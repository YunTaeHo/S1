
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "Interface/TeleportInterface.h"
#include "S1TeleportBaseBot.generated.h"

UENUM(BlueprintType)
enum class EWakinGhost : uint8
{
    Default,
    BasicAttack,
    BarrageAttack
};

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
