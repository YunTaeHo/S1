
// Copyright (c) S1

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SpawnProjectile.generated.h"

class AS1Bullet;

UCLASS()
class S1_API UAN_SpawnProjectile : public UAnimNotify
{
    GENERATED_BODY()

public:
    UAN_SpawnProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


    UPROPERTY(EditDefaultsOnly, Category = "S1|Bullet")
    TSubclassOf<AS1Bullet> BulletFactory;

    UPROPERTY(EditAnywhere)
    FName BoneSocket;
}; 
