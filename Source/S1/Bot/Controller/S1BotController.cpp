
// Copyright (c) S1

#include "S1BotController.h"
#include "Character/S1BotCharacter.h"
#include "S1LogChannel.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Player/S1BotPlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1BotController)

AS1BotController::AS1BotController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
	, StateKeyName(FName("State"))
	, PointOfInterestKeyName(FName("PointOfInterest"))
	, TargetEnemyKeyName(FName("TargetEnemy"))
	, AttackRadiusKeyName(FName("AttackRadius"))
	, DefendRadiusKeyName(FName("DefendRadius"))
{
	bWantsPlayerState = true;
	bStopAILogicOnUnposses = false;

	AIPerception = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerception"));
}

void AS1BotController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AS1BotController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AS1BotCharacter* Bot = Cast<AS1BotCharacter>(InPawn))
	{
		UBehaviorTree* BTAsset = Bot->GetBehaviorTree();
		check(BTAsset);

		RunBehaviorTree(BTAsset);		
		SetStateAsPassive();

		FIdealRange IdealRange = Bot->GetIdealRange();

		Blackboard->SetValueAsFloat(AttackRadiusKeyName, IdealRange.AttackRadius);
		Blackboard->SetValueAsFloat(DefendRadiusKeyName, IdealRange.DefendRadius);		
	}
	else
	{
		UE_LOG(LogS1, Error, TEXT("Bot OnPossess : No Valid Behavior Tree in Enemy Actor"));
	}
	
}

void AS1BotController::OnUnPossess()
{
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(CheckForgottenActosTimer);

	Super::OnUnPossess();
}

// @TODO HORK 이 부분 for문 3번이나 돌림.. 비효율적 나중에 수정하도록 하자
void AS1BotController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

}

void AS1BotController::InitPlayerState()
{
	if (!LastSeenPlayerState)
	{
		if (PlayerStateClassToSpawn)
		{
			PlayerState = GetWorld()->SpawnActor<AS1BotPlayerState>(PlayerStateClassToSpawn);
		}
		else
		{
			PlayerState = GetWorld()->SpawnActor<AS1BotPlayerState>(AS1BotPlayerState::StaticClass());
		}
		
	}

	LastSeenPlayerState = PlayerState;
}

void AS1BotController::CleanupPlayerState()
{
	Super::CleanupPlayerState();

	LastSeenPlayerState = PlayerState;
}

bool AS1BotController::CanSenseActor(AActor* PerceptionActor, EAISense Sense, FAIStimulus& Stimulus)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;

	// 시물레이션 정보를 가져온다
	AIPerception->GetActorsPerception(PerceptionActor, PerceptionInfo);


	TSubclassOf<UAISense> CurrentAISense;

	// 들어온 Enum의 기준으로 CurrentAISense를 활성화한다.
	switch (Sense)
	{
	case EAISense::Sight:
		CurrentAISense = UAISense_Sight::StaticClass();
		break;
	case EAISense::Hearing:
		CurrentAISense = UAISense_Hearing::StaticClass();
		break;
	case EAISense::Damage:
		CurrentAISense = UAISense_Damage::StaticClass();
		break;
	case EAISense::None:
	default:
		break;
	}


	// 가져온 시뮬레이션 정보들을 순회하며
	for (const FAIStimulus& AIStimulus : PerceptionInfo.LastSensedStimuli)
	{
		// 해당 시뮬레이션의 Sense 메타 데이터를 가져오고
		TSubclassOf<UAISense> StimulusAISense = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

		// AISense가 해당 Sense라면?
		if (CurrentAISense == StimulusAISense && AIStimulus.WasSuccessfullySensed())
		{
			// 시뮬레이션에 대한 정보를 전달하고, true를 반환한다
			Stimulus = AIStimulus;
			return true;
		}
	}

	// 아무 Sense도 발생하지 않았기 때문에 감지를 못하도록 막아주자
	return false;
}

