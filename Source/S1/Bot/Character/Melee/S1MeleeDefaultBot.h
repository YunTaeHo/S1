
// Copyright (c) S1

#pragma once

#include "Bot/Character/S1EquipmentDefaultBot.h"
#include "S1MeleeDefaultBot.generated.h"

/** foward declarations */
class ASeekSightBotController;
class UAnimMontage;

UCLASS()
class S1_API AS1MeleeDefaultBot : public AS1EquipmentDefaultBot, public IBlockInterface
{
    GENERATED_BODY()

public:
    AS1MeleeDefaultBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

public:
    /*
     * IBlockInterface`s Interface
     */
    virtual void TryToBlock() override;

    UFUNCTION(BlueprintCallable)
    virtual void StartBlcok(EBlockingState BlockingState) override;
    virtual void Blocking(EBlockingState BlockingState) override;
    
    UFUNCTION(BlueprintCallable)
    virtual void EndBlcok() override;


    /*
     * AS1BotCharacter`s Interface
     */
    virtual void DamageOnEvent(AActor* DamageCursor, FDamageInfo Info) override;
    virtual void Attack(AActor* Target) override;
    virtual void HitReact(EHitResponse HitResponse, AActor* DamageCursor) override;

public:
    UFUNCTION(BlueprintImplementableEvent)
    void PlayBlockStartMontage();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayBlockMontage();

    UFUNCTION(BlueprintCallable)
    virtual void CallOnBlockEnd();

    UPROPERTY(BlueprintAssignable)
    FOnBlockEnded OnBlockEnded;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Controller")
    TObjectPtr<UAnimMontage> BlockMontage;

    UPROPERTY(BlueprintReadOnly, Category = "S1|Controller")
    TObjectPtr<ASeekSightBotController> SeekSightBotController;

    UPROPERTY(EditDefaultsOnly, Category = "S1|Block")
    float BlockChance = 0.3f;

    UPROPERTY(EditDefaultsOnly, Category = "S1|Block")
    float BlockTime = 2.f;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "S1|Block")
    float AttackRange = 200.f;

    UPROPERTY(BlueprintReadOnly, Category = "S1|Block")
    float AttackRadius = 20.f;

    FTimerHandle HoldBlockTimer;

}; 
