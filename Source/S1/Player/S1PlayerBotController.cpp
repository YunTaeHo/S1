﻿
// Copyright (c) S1

#include "S1PlayerBotController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "S1LogChannel.h"
#include "GameFramework/PlayerState.h"
#include "Perception/AIPerceptionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerBotController)

AS1PlayerBotController::AS1PlayerBotController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	bWantsPlayerState = true;
	bStopAILogicOnUnposses = false;
}

void AS1PlayerBotController::OnUnPossess()
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

	Super::OnUnPossess();
}

void AS1PlayerBotController::InitPlayerState()
{
	LastSeenPlayerState = PlayerState;
}

void AS1PlayerBotController::CleanupPlayerState()
{
	LastSeenPlayerState = PlayerState;
}


