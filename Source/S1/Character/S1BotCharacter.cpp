
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
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ClientUserWidget.h"
#include "Animation/AnimInstance.h"
#include "Bot/S1BotCombatSystemComponent.h"
#include "Combat/CombatStatics.h"
#include "BrainComponent.h"

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

	// Head 충돌체 생성
	Head = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Head"));
	Head->SetCollisionProfileName(TEXT("Enemy"));
	Head->SetupAttachment(GetMesh());

	// Body 충돌체 생성 
	Body = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Body"));
	Body->SetCollisionProfileName(TEXT("Enemy"));
	Body->SetupAttachment(GetMesh());

	// Widget(3D) 생성
	Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Widget"));
	Widget->SetupAttachment(GetMesh());
}

void AS1BotCharacter::BeginPlay()
{
	Super::BeginPlay();

	BotController = Cast<AS1BotController>(GetController());

	// Enemy의 Widget을 추가해주도록 하자
	if (HeathBarWidget)
	{
		UClientUserWidget* WidgetTemp = CreateWidget<UClientUserWidget>(GetWorld(), HeathBarWidget);
		WidgetTemp->SetOwner(this);

		Widget->SetWidget(WidgetTemp);
	}
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

int32 AS1BotCharacter::GetTeamNumber()
{
	return BotCombatSystemComponent->GetTeamNumber();
}

bool AS1BotCharacter::IsDead()
{
	return BotCombatSystemComponent->IsDead();
}

void AS1BotCharacter::SetDead(bool bDead)
{
	BotCombatSystemComponent->SetDead(bDead);
}

void AS1BotCharacter::Die()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	BotController->GetBrainComponent()->StopLogic(TEXT("Bot Character Die"));
	BotController->SetStateAsDead();
}

void AS1BotCharacter::Attack(AActor* AttackTarget)
{
	if (bIsAttackStart)
	{
		bCanAttack = true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AS1BotCharacter::CallOnAttackEnd);
	}
}

void AS1BotCharacter::HitReact(EHitResponse HitResponse, AActor* DamageCursor)
{
	if (BotCombatSystemComponent->IsInterrupt())
	{
		GetCharacterMovement()->StopMovementImmediately();
		BotController->SetStateAsFrozen();
		PlayHitMontage(DamageCursor);
	}
}

void AS1BotCharacter::StartAttack(AActor* AttackTarget, int32 TokensNeeded)
{
	if (US1CombatSystemComponent* Combat = AttackTarget->FindComponentByClass<US1CombatSystemComponent>())
	{
		Combat->ReserveAttackToken(TokensNeeded);
		TokensUseInCurrentAttack = TokensNeeded;
		bIsAttackStart = true;
	}
	else
	{
		bIsAttackStart = false;
	}
}

void AS1BotCharacter::EndAttack(AActor* AttackTarget, int32 TokensNeeded)
{
	if (US1CombatSystemComponent* Combat = AttackTarget->FindComponentByClass<US1CombatSystemComponent>())
	{
		Combat->ReturnAttackToken(TokensNeeded);
	}

	CallOnAttackEnd();
}

void AS1BotCharacter::CallOnAttackEnd()
{
	OnAttackEnded.Broadcast();
	TokensUseInCurrentAttack = 0;
	bCanAttack = false;
	bIsAttackStart = false;
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

void AS1BotCharacter::OnLand(const FHitResult& Hit)
{
	LandedDelegate.Remove(this, TEXT("OnLand"));
	GetCharacterMovement()->StopMovementImmediately();
}

void AS1BotCharacter::UseLandDelegate()
{
	LandedDelegate.AddDynamic(this, &AS1BotCharacter::OnLand);
}


void AS1BotCharacter::SetMovementSpeed(EMovementState MoveState)
{
	switch (MoveState)
	{
	case EMovementState::Idle:
		GetCharacterMovement()->MaxWalkSpeed = MovementState.IdleSpeed;
		break;
	case EMovementState::Walking:
		GetCharacterMovement()->MaxWalkSpeed = MovementState.WalkSpeed;
		break;
	case EMovementState::Jogging:
		GetCharacterMovement()->MaxWalkSpeed = MovementState.JogSpeed;
		break;
	case EMovementState::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = MovementState.SprintSpeed;
		break;
	default:
		break;
	}
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