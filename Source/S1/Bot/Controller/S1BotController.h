
// Copyright (c) S1

#pragma once

#include "ModularAIController.h"
#include "Misc/EnumAI.h"
#include "S1BotController.generated.h"

/** foward declarations */
class UAIPerceptionComponent;
class UBehaviorTree;
class AS1BotPlayerState;
class UAISenseConfig;
struct FAIStimulus;


UCLASS()
class S1_API AS1BotController : public AModularAIController
{
    GENERATED_BODY()

public:
    AS1BotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void PreInitializeComponents() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;


protected:
    virtual void InitPlayerState() override;
    virtual void CleanupPlayerState() override;

/** State 변경 함수*/
public:
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsPassive() {};
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsAttacking(AActor* TargetActor) {};
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsInvestigating(FVector Location) {};
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsFrozen() {};
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsSkeeing(FVector Location) {};
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void SetStateAsDead() {};

 
public:
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual EAIState GetCurrentState() const { return EAIState::Passive; }

public:
    /** 센서 감지 함수, Update Perception 함수에서 사용*/
    /** @TODO HORK 이 부분 for문 3번이나 돌림..비효율적 나중에 수정하도록 하자 */
    virtual bool CanSenseActor(AActor* PerceptionActor, EAISense Sense, FAIStimulus& Stimulus);

    virtual void HandleSensedSight(AActor* PerceptionActor) {}
    virtual void HandleSensedSound(FVector LocationAtSound) {}
    virtual void HandleSensedDamage(AActor* PerceptionActor) {}


protected:
    /** AI Perception */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TObjectPtr<UAIPerceptionComponent> AIPerception;

    /** 어떤 적을 추적할 지 확인용도 변수 */
    UPROPERTY(BlueprintReadWrite, Category = "S1|AI")
    TObjectPtr<AActor> TargetEnemy;


/** BlbackBoard에서 사용할 Key(Name)값 */
protected:
    /** State */
    UPROPERTY(BlueprintReadWrite, Category = "S1|Keys")
    FName StateKeyName;

    /** Point(EQS) */
    UPROPERTY(BlueprintReadWrite, Category = "S1|Keys")
    FName PointOfInterestKeyName;

    /** TargetEnemy */
    UPROPERTY(BlueprintReadWrite, Category = "S1|Keys")
    FName TargetEnemyKeyName;

    UPROPERTY(BlueprintReadWrite, Category = "S1|Keys")
    FName AttackRadiusKeyName;

    UPROPERTY(BlueprintReadWrite, Category = "S1|Keys")
    FName DefendRadiusKeyName;
    

protected:
    /** AI에 사용할 컴포넌트 */
    UPROPERTY(EditDefaultsOnly, Category = "S1|PlayerState")
    TSubclassOf<AS1BotPlayerState> PlayerStateClassToSpawn;

    /** 저장용 PlayerState */
    UPROPERTY()
    TObjectPtr<APlayerState> LastSeenPlayerState;

    /** 테스트 */
    FTimerHandle CheckForgottenActosTimer;

}; 
