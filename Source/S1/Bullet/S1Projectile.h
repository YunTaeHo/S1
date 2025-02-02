
// Copyright (c) S1

#pragma once

#include "GameFramework/Actor.h"
#include "S1Projectile.generated.h"

/*
 * HitScan이 아닌 투사체들을 사용할 때 사용
 *  - Projectile을 사용해서 떨어질 때 사용
 */
UCLASS()
class S1_API AS1Projectile : public AActor
{
    GENERATED_BODY()

public:
    AS1Projectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



}; 
