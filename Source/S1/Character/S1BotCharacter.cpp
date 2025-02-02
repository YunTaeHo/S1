
// Copyright (c) S1

#include "S1BotCharacter.h"
#include "S1LogChannel.h"
#include "S1HealthComponent.h"
#include "Player/S1BotPlayerState.h"
#include "Player/S1PlayerBotController.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1BotCharacter)

AS1BotCharacter::AS1BotCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// HealthComponent 생성
	{
		HealthComponent = CreateDefaultSubobject<US1HealthComponent>(TEXT("HealthComponent"));
	}

}

void AS1BotCharacter::BeginPlay()
{
	Super::BeginPlay();
}


UAbilitySystemComponent* AS1BotCharacter::GetAbilitySystemComponent() const
{
	// 캐싱되어있는 ASC를 가져와서 return 해준다
	AS1PlayerBotController* AIC = Cast<AS1PlayerBotController>(GetController());
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

