
// Copyright (c) S1

#include "S1BotPlayerState.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/S1CombatSet.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1BotPlayerState)

AS1BotPlayerState::AS1BotPlayerState(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<US1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));

    // AbilitySystemComponent 내부에 자동적으로 캐싱을 수행한다
    CreateDefaultSubobject<US1CombatSet>(TEXT("CombatSet"));
    CreateDefaultSubobject<US1HealthSet>(TEXT("HealthSet"));
}

void AS1BotPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    check(AbilitySystemComponent);
    // InitAbilityActorInfo를 재호출
    {
        // 처음 InitAbilityActorInfo 호출 당시, OwnerACtor와 AvatarActor가 같은 Actor를 가르키고 있으며, 이는 PlayerState이다
        // - OwnerActor -> PlayerState, AvatarActor -> Pawn이 되야 하기 때문에 재-세팅을 수행한다
        FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
        check(ActorInfo->OwnerActor == this);
        check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
    }

    // AvatarActor를 nullptr로 시켜준다(의도하는 결과에서 벗어나지 않게 하기 위해)
    AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

}


