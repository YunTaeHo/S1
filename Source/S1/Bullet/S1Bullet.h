
// Copyright (c) S1

#pragma once

#include "Templates/SubclassOf.h"
#include "GameFramework/Actor.h"
#include "S1Bullet.generated.h"

/** foward declarations */
class USphereComponent;
class URotatingMovementComponent;
class UProjectileMovementComponent;
struct FDamageInfo;

/*
 * HitScan이 아닌 투사체들을 사용할 때 사용
 */
UCLASS()
class S1_API AS1Bullet : public AActor
{
    GENERATED_BODY()

public:
    AS1Bullet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    static AS1Bullet* SpawnLinear(AActor* WorldContextObject, FTransform SpawnTransform, TSubclassOf<AS1Bullet> BulletFactory, FDamageInfo InDamageInfo, FBulletInfo InBulletInfo);

    // Bullet을 어떻게 구현할 것인가?
    // 1. 타겟이 있다면 해당 위치로 러프하게 날아간다?
    //      - 이후 타겟을 지나쳤다면 그냥 쭉 날아간다(지나쳤다는것을 어떻게?)
    //      - 아니면 몇 초동안 타겟위치로 날아가다가 그 방향으로 쭉 직진하게 한다
    //      - 그냥 타겟 방향으로 쏘고 Bullet을 부드럽게 회전시켜주자(Cos)     
    // 
    // 2. 타겟이 없다면 쭉 날아간다
    // 그럼 필요한게 타겟, Direction? 일단 이정도가 필요해보인다
    
    // 플레이어도 Bullet을 쏠 수 있는데, 그 때 필요한 것도 추가해보자
    // 현재는 딱히 추가할 게 없어보인다(

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(BlueprintReadOnly)
    AActor* Target;

    FVector Direction;

    UPROPERTY()
    FDamageInfo DamageInfo;

    /** 언제 삭제 될 것인지에 대한 타이머 */
    UPROPERTY(EditDefaultsOnly, Category = "S1|Damage")
    float DelayBeforeDestroying = 0.1f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Collider")
    TObjectPtr<USphereComponent> ProjectileCollider;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Movement")
    TObjectPtr<URotatingMovementComponent> RotatingMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Movement")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
    UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent)
    void EffectHits(AActor* OtherActor, const FHitResult& SweepResult);

    UFUNCTION()
    void OnProjectileHit(const FHitResult& Hit);
}; 
