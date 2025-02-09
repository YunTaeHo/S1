
// Copyright (c) S1

#pragma once

#include "S1BotController.h"
#include "SeekSightBotController.generated.h"

UCLASS()
class S1_API ASeekSightBotController : public AS1BotController
{
    GENERATED_BODY()

public:
    ASeekSightBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;


/** State 변경 함수 */
public:
    virtual void SetStateAsPassive() override;
    virtual void SetStateAsAttacking(AActor* TargetActor, bool UseLastKnownAttackTarget) override;
    virtual void SetStateAsInvestigating(FVector Location) override;
    virtual void SetStateAsFrozen() override;
    virtual void SetStateAsSkeeing(FVector Location) override;
    virtual void SetStateAsDead() override;


public:
    virtual EAIState GetCurrentState() const override;

public:
    virtual void HandleSensedSight(AActor* PerceptionActor) override;
    virtual void HandleSensedSound(FVector LocationAtSound) override;
    virtual void HandleSensedDamage(AActor* PerceptionActor) override;

    void HandleForgotActor(AActor* TargetActor);
    void HandleLostSight(AActor* TargetActor);
    void SeekAttackTarget();

protected:
    /** Seek 시점을 Target을 정해준다*/
    void CheckForgettenSeenActor();

protected:
    /** 현재 내가 보고 있는 액터들(Sight Sense 용도) */
    UPROPERTY(BlueprintReadWrite, Category = "S1|Sense")
    TArray<TObjectPtr<AActor>> KnownSeenActors;

    /** 몇 초 동안 LosingShight를 확인할 것인지에 대한 체크 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Sense")
    float TimeToSeekAfterLosingShight;

private:
    FTimerHandle SeekAttackTargetTimer;
}; 
