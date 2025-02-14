
// Copyright (c) S1

#pragma once

#include "Bot/Character/Base/S1EquipmentBlockBaseBot.h"
#include "S1MeleeDefaultBot.generated.h"

UENUM(BlueprintType)
enum class ELiosAttack : uint8
{
    Default,
    ShortRangeAttack,
    LongRangeAttack,
    SmashAttack,
    SpinningAttack,
};

/** foward declarations */
class ASeekSightBotController;
class UAnimMontage;

UCLASS()
class S1_API AS1MeleeDefaultBot : public AS1EquipmentBlockBaseBot
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
    virtual void HitReact(EHitResponse HitResponse, AActor* DamageCursor) override;

    /** 블록 관련 함수*/
public:
    UFUNCTION(BlueprintImplementableEvent)
    void PlayBlockStartMontage();

    UFUNCTION(BlueprintImplementableEvent)
    void PlayBlockMontage();

    UFUNCTION(BlueprintCallable)
    virtual void CallOnBlockEnd();

    UPROPERTY(BlueprintAssignable)
    FOnBlockEnded OnBlockEnded;

    /** JumpAttack 관련 함수 */
public:
    /** 공격 상대가 가까울 때 가만히 위에서 내려 찍는다 */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ShortRangeAttack(AActor* AttackTarget);

    /** 공격 상대가 멀 때 상대 위치를 예측하면서 내리 찍는다 */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void LongRangeAttack(AActor* AttackTarget);

    /** 지면 공격을 수행한다  */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SmashAttack(AActor* AttackTarget);

protected:
    /** 타겟을향해 점프 공격을 수행시켜주자 */
    UFUNCTION(BlueprintCallable)
    void JumpToAttackTarget(AActor* Target);

private:
    FVector CalculateFutureActorLocation(AActor* Actor, float Time);

    /** Spin 관련 함수 */
public:
    /** Event로 PlayMontage를 이용해 회전 공격을 수행하자 */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SpiningAttack(AActor* AttackTarget);

protected:
    /** 메쉬만 회전시켜주면서 회전하면서 공격하는 것 처럼 보이게 하자 */
    void SpinMesh();

    /** 메쉬를 회전시키기 위한 타이머 세팅 */
    UFUNCTION(BlueprintCallable)
    void SpinStart();

    UFUNCTION(BlueprintCallable)
    void ChaseTarget(AActor* Target);


protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "S1|Attack")
    float SpiningTime = 4.f;

    UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
    FRotator MeshRotationBeforeSpin;

    float SpinCurrentTime;


protected:
    UPROPERTY(BlueprintReadOnly, Category = "S1|Controller")
    TObjectPtr<ASeekSightBotController> SeekSightBotController;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Controller")
    TObjectPtr<UAnimMontage> BlockMontage;

    /** 공격 방어 확률 */
    UPROPERTY(EditDefaultsOnly, Category = "S1|Block")
    float BlockChance = 0.3f;

    /** 공격 방어 시간 */
    UPROPERTY(EditDefaultsOnly, Category = "S1|Block")
    float BlockTime = 2.f;


protected:
    /** 공격 길이*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "S1|Block")
    float AttackRange = 200.f;

    /** 공격 범위 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Block")
    float AttackRadius = 20.f;


    FTimerHandle HoldBlockTimer;

}; 
