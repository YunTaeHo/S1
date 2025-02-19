
// Copyright (c) S1

#pragma once

#include "Bot/Character/S1EquipmentDefaultBot.h"
#include "Interface/TeleportInterface.h"
#include "S1Laien.generated.h"

UENUM(BlueprintType)
enum class ES1LaienState : uint8
{
    BasicAttack,
    KnockbackAttack,
    JumpAttack,
    ThrowAttack,
    BossSkill
};

UCLASS()
class S1_API AS1Laien : public AS1EquipmentDefaultBot, public ITeleportInterface
{
    GENERATED_BODY()

public:
    AS1Laien(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
    /** ITeleportInterface */
    UFUNCTION(BlueprintCallable)
    virtual void Teleport(FVector Location) override;

    UFUNCTION(BlueprintImplementableEvent)
    void TeleporStart(FVector Location, AActor* Target);

    UFUNCTION(BlueprintCallable)
    void TeleporEnd();

    UFUNCTION(BlueprintImplementableEvent)
    void DestroyedEffect();

    UPROPERTY(BlueprintAssignable)
    FOnTeleportEnd OnTeleportEnd;

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void RushAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void DefaultAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void AOEAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ThrowAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void JumpAttack(AActor* Target);


}; 
