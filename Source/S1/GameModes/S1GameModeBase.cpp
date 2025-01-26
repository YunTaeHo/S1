// Fill out your copyright notice in the Description page of Project Settings.


#include "S1GameModeBase.h"
#include "S1GameState.h"
#include "S1ExperienceManagerComponent.h"
#include "S1ExperienceDefinition.h"
#include "Player/S1PlayerController.h"
#include "Player/S1PlayerState.h"
#include "Character/S1Character.h"
#include "Character/S1PawnData.h"
#include "Character/S1PawnHandler.h"
#include "S1LogChannel.h"
#include "Kismet/GameplayStatics.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameModeBase)

AS1GameModeBase::AS1GameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AS1GameState::StaticClass();
	PlayerControllerClass = AS1PlayerController::StaticClass();
	PlayerStateClass = AS1PlayerState::StaticClass();
	DefaultPawnClass = AS1Character::StaticClass();

}

void AS1GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// GameInstance를 통해 초기화 작업이 진행되므로, 현 프레임에는 Experience 처리가 불가능
	//	- 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNoExpectingOne);
}

void AS1GameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 Delegate 준비
	US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallorRegister_OnExperienceLoaded(FOnS1ExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

APawn* AS1GameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(SpawnedPawn))
			{
				if (const US1PawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnHandler->SetPawnData(PawnData);
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;

		}
	}

	return nullptr;
}

void AS1GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// 로딩이 되어있다면?
	if (IsExperienceLoaded())
	{
		// Super 안에 있는 RestartPlayer를 호출함
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

UClass* AS1GameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 이용하여, Pawn을 가져오자
	if (const US1PawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// 해당 함수에서는 우리가 로딩할 Expierience에 대해 PrimaryAssetId를 생성하먀, OnMtachAssignmentGive으로 넘겨준다
PRAGMA_DISABLE_OPTIMIZATION
void AS1GameModeBase::HandleMatchAssignmentIfNoExpectingOne()
{
	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();

	// URL과 함께 ExtraArgs로 넘겼던 정보는 OptionStrint에도 저장되어 있다
	//	- 로딩 정보들을 Option을 Parse해서 사용하도록 하자
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience의 Value를 가져와서 PrimaryAssetId를 생성해준다
		//	- S1ExperienceDefintion의 Class 이름을 사용
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(US1ExperienceDefinition::StaticClass()->GetFName(), FName(*ExperienceFromOptions));
	}

	// Experience가 존재하지 않는다면 Default Setting으로 설정
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId("S1ExperienceDefinition", FName("B_S1DefaultExperience"));
	}


	check(ExperienceId.IsValid());

	//ExperienceManager를 불러와 ServerSetCurrentExperience를 호출시켜준다
	US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->SetCurrentExperience(ExperienceId);
}
PRAGMA_ENABLE_OPTIMIZATION

bool AS1GameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AS1GameModeBase::OnExperienceLoaded(const US1ExperienceDefinition* CurrentExperience)
{
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	// PC가 Pawn를 Possess하지 않았다면 RestartPlayer를 통해 Pawn을 다시 Spawn한다
	if (PC && PC->GetPawn() == nullptr)
	{
		if (PlayerCanRestart(PC))
		{
			RestartPlayer(PC);
		}
	}
}

const US1PawnData* AS1GameModeBase::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중 PawnData가 override 되었을 경우, PawnData는 PlayerState에서 가져온다
	if (InController)
	{
		if (const AS1PlayerState* S1PS = InController->GetPlayerState<AS1PlayerState>())
		{
			if (const US1PawnData* PawnData = S1PS->GetPawnData<US1PawnData>())
			{
				return PawnData;
			}
		}
	}

	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정
	check(GameState);
	US1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<US1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const US1ExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스도 잡히지 않았다면 nullptr 반환
	return nullptr;
}
