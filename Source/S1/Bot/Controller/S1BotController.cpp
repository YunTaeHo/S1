
// Copyright (c) S1

#include "S1BotController.h"
#include "Bot/BoInterface.h"
#include "Character/S1BotCharacter.h"
#include "S1LogChannel.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Player/S1BotPlayerState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/S1BotCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
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

		if (IBoInterface* BotInterface = Cast<IBoInterface>(Bot))
		{
			FIdealRange IdealRange = BotInterface->GetIdealRange();

			Blackboard->SetValueAsFloat(AttackRadiusKeyName, IdealRange.AttackRadius);
			Blackboard->SetValueAsFloat(DefendRadiusKeyName, IdealRange.DefendRadius);
		}
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

void AS1BotController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

}

void AS1BotController::InitPlayerState()
{
	if (!LastSeenPlayerState && PlayerStateClassToSpawn)
	{
		PlayerState = GetWorld()->SpawnActor<AS1BotPlayerState>(PlayerStateClassToSpawn);
	}

	LastSeenPlayerState = PlayerState;
}

void AS1BotController::CleanupPlayerState()
{
	Super::CleanupPlayerState();

	LastSeenPlayerState = PlayerState;
}


