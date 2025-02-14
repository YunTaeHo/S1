
// Copyright (c) S1

#include "S1MeleeDefaultBot.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include "Bot/Controller/S1BotController.h"
#include "Bot/S1BotCombatSystemComponent.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "Combat/CombatStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1MeleeDefaultBot)

AS1MeleeDefaultBot::AS1MeleeDefaultBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1MeleeDefaultBot::BeginPlay()
{
	Super::BeginPlay();

	SeekSightBotController = GetController<ASeekSightBotController>();
}

void AS1MeleeDefaultBot::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorld()->GetTimerManager().ClearTimer(HoldBlockTimer);

	Super::EndPlay(Reason);
}

void AS1MeleeDefaultBot::DamageOnEvent(AActor* DamageCursor, FDamageInfo Info)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	if (Info.DamageEffect)
	{
		FGameplayEffectContextHandle EffectContext;
		ASC->BP_ApplyGameplayEffectToSelf(Info.DamageEffect, Info.Level, EffectContext);
		BotController->SetStateAsFrozen();

		// ASC에서 HealthSet을 가져와 Hp를 가져온다
		const US1HealthSet* HealthSet = ASC->GetSet<US1HealthSet>();
		float CurHp = HealthSet->GetHealth();

		if (CurHp > 0.f)
		{
			// 만약 날라오는 탄창을 맞았는데, 블록이 가능하다면?
			if (Info.bCanBlocked)
			{
				// 블록을 시도함
				TryToBlock();

			}

			//GetMesh()->GetAnimInstance()->Montage_Play();
			BotController->SetStateAsAttacking(DamageCursor, false);
			HitReact(Info.HitResponse, DamageCursor);
		}
		else
		{
			Die();
		}
	}

}
void AS1MeleeDefaultBot::HitReact(EHitResponse HitResponse, AActor* DamageCursor)
{
	GetCharacterMovement()->StopMovementImmediately();

	if (BlockState == EBlockingState::Block)
	{
		if (HitResponse == EHitResponse::HitReaction || HitResponse == EHitResponse::None)
		{
			PlayBlockMontage();
			Blocking(EBlockingState::BlockSuccessfully);
		}
		else if (HitResponse == EHitResponse::KnockBack)
		{
			//Blocking(EBlockingState::BlockKnockBack);
		}
	}
	else
	{
		if (BotCombatSystemComponent->IsInterrupt())
		{
			PlayHitMontage(DamageCursor);
		}

	}
}

void AS1MeleeDefaultBot::TryToBlock()
{
	if (FMath::RandRange(0.0f, 1.f) <= BlockChance)
	{
		StartBlcok(EBlockingState::None);
	}
}

void AS1MeleeDefaultBot::StartBlcok(EBlockingState BlockingState)
{
	GetWorld()->GetTimerManager().ClearTimer(HoldBlockTimer);
	GetCharacterMovement()->StopMovementImmediately();
	BlockState = EBlockingState::Block;
	BotCombatSystemComponent->SetBlock(true);
	GetWorld()->GetTimerManager().SetTimer(HoldBlockTimer, this, &AS1MeleeDefaultBot::EndBlcok, BlockTime, false);
	PlayBlockStartMontage();
}

void AS1MeleeDefaultBot::Blocking(EBlockingState BlockingState)
{
	if (BlockingState == EBlockingState::BlockSuccessfully)
	{
		BlockState = EBlockingState::BlockSuccessfully;
		GetWorld()->GetTimerManager().ClearTimer(HoldBlockTimer);

		UAnimInstance* Anim = GetMesh()->GetAnimInstance();

		PlayBlockMontage();
	}
}

void AS1MeleeDefaultBot::EndBlcok()
{
	GetWorld()->GetTimerManager().ClearTimer(HoldBlockTimer);
	BotCombatSystemComponent->SetBlock(false);
	BlockState = EBlockingState::None;
	CallOnBlockEnd();
}

void AS1MeleeDefaultBot::CallOnBlockEnd()
{
	OnBlockEnded.Broadcast();
}

void AS1MeleeDefaultBot::JumpToAttackTarget(AActor* Target)
{
	UCombatStatics::JumpToVelocity(this, GetActorLocation(), CalculateFutureActorLocation(Target, 1.f));
}

FVector AS1MeleeDefaultBot::CalculateFutureActorLocation(AActor* Actor, float Time)
{
	return Actor->GetActorLocation() + Actor->GetVelocity() * FVector(1.0, 1.0, 0.0) * Time;
}

void AS1MeleeDefaultBot::SpinMesh()
{
	if (UWorld* World = GetWorld())
	{
		SpinCurrentTime += World->DeltaTimeSeconds;
		USkeletalMeshComponent* MeshComp = GetMesh();
		FRotator Rot = MeshComp->GetRelativeRotation();

		float Yaw = FMath::Lerp(Rot.Yaw, Rot.Yaw + 4320.f, SpinCurrentTime / SpiningTime);

		MeshComp->SetRelativeRotation(FRotator(Rot.Roll, Rot.Pitch, Yaw));
	}
}

void AS1MeleeDefaultBot::SpinStart()
{
	FTimerHandle Handle;
	SpinCurrentTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AS1MeleeDefaultBot::SpinMesh, SpiningTime, true);
}

void AS1MeleeDefaultBot::ChaseTarget(AActor* Target)
{
	FAIMoveRequest Request;
	Request.SetGoalActor(Target);
	Request.SetAcceptanceRadius(200.f);
	FNavPathSharedPtr Ptr;
	BotController->MoveTo(Request, &Ptr);
}


//void AS1MeleeDefaultBot::NotfiyOnSlashBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
//{
//	if (NotifyName == TEXT("Slash"))
//	{
//		FVector StartPos = GetActorLocation();
//		FVector EndPos = StartPos + GetActorForwardVector() * AttackRange;
//
//		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
//		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
//
//		TArray<AActor*> ActorToIgnore;
//		TArray<FHitResult> Hits;
//
//		if (UKismetSystemLibrary::SphereTraceMultiForObjects
//		(
//			GetWorld(),
//			StartPos,
//			EndPos,
//			AttackRadius,
//			ObjectTypes,
//			false,
//			ActorToIgnore,
//			EDrawDebugTrace::Type::ForDuration,
//			Hits,
//			true
//		))
//		{
//			UCombatStatics::DamageAllNonTeamMembers(this, Hits, DamageInfos[0]);
//		}
//	}
//}



