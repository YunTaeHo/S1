
// Copyright (c) S1

#include "S1PlayerState.h"
#include "GameModes/S1ExperienceDefinition.h"
#include "GameModes/S1ExperienceManagerComponent.h"
#include "GameModes/S1GameModeBase.h"
#include "Character/S1PawnData.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "AbilitySystem/S1AbilitySet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerState)

// 기본 생성자
AS1PlayerState::AS1PlayerState(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<US1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void AS1PlayerState::PostInitializeComponents()
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

    // 월드 기반으로 GameState를 가져오고
    AGameStateBase* GameState = GetWorld()->GetGameState();
    check(GameState);

    // ExperienceManager를 가져온 뒤
    US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
    check(ExperienceManagerComponent);

    // 로딩 완료 델리게이트를 등록시켜준다
    ExperienceManagerComponent->CallorRegister_OnExperienceLoaded(FOnS1ExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AS1PlayerState::SetPawnData(const US1PawnData* InPawnData)
{
    check(InPawnData);

    // PawnData가 두 번 설정되지 않도록 체크
    check(!PawnData);

    PawnData = InPawnData;

    // PawnData의 AbilitySet을 순회하며, ASc에 Abiltiy를 할당(Give)한다
    // - 이 과정에서 ASC의 ActivateableAbilities에 추가된다
    for (US1AbilitySet* AbilitySet : PawnData->AbilitySets)
    {
        if (AbilitySet)
        {
            AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
        }
    }
}

void AS1PlayerState::OnExperienceLoaded(const US1ExperienceDefinition* CurrentExperience)
{
    if (AS1GameModeBase* GameMode = GetWorld()->GetAuthGameMode<AS1GameModeBase>())
    {
        // GetPawnDataForController에서 PawnData를 반환한다
        const US1PawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
        check(NewPawnData);

        SetPawnData(NewPawnData);
    }
}


