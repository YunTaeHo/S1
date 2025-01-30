
// Copyright (c) S1

#include "S1AnimInstance.h"
#include "AbilitySystemGlobals.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1AnimInstance)

void US1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void US1AnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부 관리하는 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여, 값 변화에 때한 반영을 진행한다
	GameplayTagPropertyMap.Initialize(this, ASC);
}
