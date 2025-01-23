
// Copyright (c) S1

#include "S1PlayerState.h"
#include "GameModes/S1ExperienceDefinition.h"
#include "GameModes/S1ExperienceManagerComponent.h"
#include "GameModes/S1GameModeBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerState)

// 기본 생성자
AS1PlayerState::AS1PlayerState(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1PlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

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


