
// Copyright (c) S1

#pragma once

#include "GameFramework/Actor.h"
#include "S1Bullet.generated.h"

/*
 * HitScan이 아닌 투사체들을 사용할 때 사용
 */
UCLASS()
class S1_API AS1Bullet : public AActor
{
    GENERATED_BODY()

public:
    AS1Bullet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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
    
    void SetTarget(AActor* NewTarget) { Target = NewTarget; }

protected:
    UPROPERTY()
    AActor* Target;

    FVector Direction;
}; 
