
// Copyright (c) S1

#include "S1PatrolRoute.h"
#include "Components/SplineComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PatrolRoute)

AS1PatrolRoute::AS1PatrolRoute(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	RatrolRoute = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("RatrolRoute"));
	SetRootComponent(RatrolRoute);
}


FVector AS1PatrolRoute::GetSplinePointAsWorldPosition()
{
	return RatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

void AS1PatrolRoute::IncrementPartolRoute()
{
	// RatrolRoute의 다음 인덱스로 갈 수 있도록 설정
	PatrolIndex += Direction;

	// PatrolIndex가 마지막에 도달했다면?
	if (PatrolIndex == RatrolRoute->GetNumberOfSplinePoints() - 1)
	{
		// 반대로 이동시킨다
		Direction = -1;
	}
	else
	{
		// PatrolIndex가 처음이라면?
		if (PatrolIndex == 0)
		{
			// 정방향으로 이동시킨다
			Direction = 1;
		}
	}
}


