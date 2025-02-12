
// Copyright (c) S1

#include "S1Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Combat/CombatStatics.h"
#include "Misc/S1Container.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1Bullet)

AS1Bullet::AS1Bullet(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ProjectileCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollider"));
    SetRootComponent(ProjectileCollider);

    RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->SetUpdatedComponent(RootComponent);
    ProjectileMovement->ProjectileGravityScale = 0.f;

}

AS1Bullet* AS1Bullet::SpawnLinear(AActor* WorldContextObject, FTransform SpawnTransform, TSubclassOf<AS1Bullet> BulletFactory, FDamageInfo InDamageInfo, FBulletInfo InBulletInfo)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    AS1Bullet* Bullet = WorldContextObject->GetWorld()->SpawnActor<AS1Bullet>(BulletFactory, SpawnTransform);
    Bullet->SetOwner(WorldContextObject);
    Bullet->Target = InBulletInfo.Target;

    Bullet->DamageInfo = MoveTemp(InDamageInfo);
    UProjectileMovementComponent* Movement = Bullet->ProjectileMovement;
    Movement->MaxSpeed = InBulletInfo.Speed;
    Movement->InitialSpeed = InBulletInfo.Speed;
    Movement->ProjectileGravityScale = InBulletInfo.Gravity;
    Movement->bIsHomingProjectile = InBulletInfo.bIsHoming;
    Movement->OnProjectileStop.AddDynamic(Bullet, &ThisClass::OnProjectileHit);

    return Bullet;
}

void AS1Bullet::BeginPlay()
{
    Super::BeginPlay();

    ProjectileCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ProjectileCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AS1Bullet::OnProjectileHit(const FHitResult& Hit)
{
    if (AActor* HitTarget = Hit.GetActor())
    {
        //UCombatStatics::ApplyDamageToTarget(GetOwner(), HitTarget, DamageInfo);
        EffectHits(HitTarget, Hit);
    }
}

void AS1Bullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* MyOnwer = GetOwner();

    UCombatStatics::ApplyDamageToTarget(OtherActor, MyOnwer, DamageInfo);
    ProjectileCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // @TODO 나중에 오브젝트 풀링 수행해야함
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]() {
            Destroy();
    }), DelayBeforeDestroying, false);
}


