
// Copyright (c) S1

#include "S1BotCharacter.h"
#include "S1LogChannel.h"
#include "S1HealthComponent.h"
#include "Player/S1BotPlayerState.h"
#include "Bot/Controller/S1BotController.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/S1GameplayAbility.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/AsyncAction_ExperienceReady.h"
#include "Bot/S1BotCombatSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1BotCharacter)

AS1BotCharacter::AS1BotCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// HealthComponent 생성
	HealthComponent = ObjectInitializer.CreateDefaultSubobject<US1HealthComponent>(this, TEXT("HealthComponent"));
	
	// BotCombatSystemComponent 생성 
	BotCombatSystemComponent = ObjectInitializer.CreateDefaultSubobject<US1BotCombatSystemComponent>(this, TEXT("BotCombatSystemComponent"));

	
}

void AS1BotCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AS1BotCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	US1AbilitySystemComponent* ASC = Cast<US1AbilitySystemComponent>(GetAbilitySystemComponent());
	check(ASC);
	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void AS1BotCharacter::DamageOnEvent(AActor* DamageCursor, FDamageInfo Info)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);

	if (Info.DamageEffect)
	{
		FGameplayEffectContextHandle EffectContext;
		ASC->BP_ApplyGameplayEffectToSelf(Info.DamageEffect, Info.Level, EffectContext);
		AS1BotController* BotController = Cast<AS1BotController>(GetController());

		check(BotController);

		BotController->SetStateAsFrozen();

		// ASC에서 HealthSet을 가져와 Hp를 가져온다
		const US1HealthSet* HealthSet = ASC->GetSet<US1HealthSet>();
		float CurHp = HealthSet->GetHealth();

		if (CurHp > 0.f)
		{
			//GetMesh()->GetAnimInstance()->Montage_Play();
			BotController->SetStateAsAttacking(DamageCursor, false);
			HitReact(Info.HitResponse, DamageCursor);
		}
		else
		{
			BotCombatSystemComponent->SetDead(true);
			Die();
		}
	}
}

void AS1BotCharacter::CallOnAttackEnd()
{
	OnAttackEnded.Broadcast();
	TokensUseInCurrentAttack = 0;
	bCanAttack = false;
	bIsAttackStart = false;
}

void AS1BotCharacter::CallOnBlockEnd()
{
	OnBlockEnded.Broadcast();
}

UAbilitySystemComponent* AS1BotCharacter::GetAbilitySystemComponent() const
{
	// 캐싱되어있는 ASC를 가져와서 return 해준다
	AS1BotController* AIC = Cast<AS1BotController>(GetController());
	//check(AIC);

	if (!AIC)
	{
		UE_LOG(LogS1, Error, TEXT("AS1PlayerBotController Is Null!"));
		return nullptr;
	}

	AS1BotPlayerState* PS = Cast<AS1BotPlayerState>(AIC->PlayerState);
	if (!PS)
	{
		UE_LOG(LogS1, Error, TEXT("AS1BotPlayerState Is Null!"));
		return nullptr;
	}


	return PS->GetS1AbilitySystemComponent();
}

void AS1BotCharacter::JumpToDestination(FVector Location)
{
	FVector NewLocation = Location;
	NewLocation.Z += ZVelocity;

	FVector OutLaunchVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), OutLaunchVelocity, GetActorLocation(), NewLocation);

	LaunchCharacter(OutLaunchVelocity, true, true);
}

void AS1BotCharacter::ReturnAttackToken(int32 Amount)
{
	BotCombatSystemComponent->ReturnAttackToken(Amount);
}