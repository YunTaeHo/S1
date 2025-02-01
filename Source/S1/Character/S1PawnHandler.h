
// Copyright (c) S1

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "S1PawnHandler.generated.h"

/** 전방 선언 */
class US1PawnData;
class US1AbilitySystemComponent;

/*
 * 기본적인 초기화를 다루는 Component
 */
UCLASS()
class S1_API US1PawnHandler : public UPawnComponent, public IGameFrameworkInitStateInterface
{
    GENERATED_BODY()

public:
    US1PawnHandler(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;

	/*
	 * member methods
	 */
	static US1PawnHandler* FindPawnHandler(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<US1PawnHandler>() : nullptr); }
	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const US1PawnData* InPawnData);
	void ForceUpdateInitState();
	void InitializeAbilitySystem(US1AbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();
	US1AbilitySystemComponent* GetS1AbilitySystemComponent() const { return AbilitySystemComponent; }

	/*
	 * UPawnComponent methods
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	/*
	 *	IGameFrameworkInitStateInterface
	 */
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	/** OnAbilitySystem[Initialized|Uninitialized] Delegate에 추가 */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	/** Pawn을 생성한 데이터를 캐싱 */
	UPROPERTY(EditInstanceOnly, Category = "S1|Pawn")
	TObjectPtr<const US1PawnData> PawnData;

	/** AbilitySystemComponent 캐싱 */
	UPROPERTY()
	US1AbilitySystemComponent* AbilitySystemComponent;

	/** ASC Init과 UnInit의 Delegate 추가 */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

}; 
