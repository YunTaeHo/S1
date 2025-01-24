
// Copyright (c) S1

#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "S1PlayerComponent.generated.h"

/** 전방 선언 */
class US1CameraMode;
template<class TClass> class TSubclassOf;
struct FS1MappableConfigPair;
struct FInputActionValue;
class UInputComponent;

/*
 *  카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class S1_API US1PlayerComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
    GENERATED_BODY()

public:
    US1PlayerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** FeatureName 정의 */
    static const FName NAME_ActorFeatureName;

    /*
     *  UPawnComponent`s Interfaces
     */
    virtual void OnRegister() final;
    virtual void BeginPlay() final;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) final;

    /*
     *  IGameFrameworkInitStateInterface
     */
    virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
    virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
    virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
    virtual void CheckDefaultInitialization() final;
    virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;

    /*
     *  member methods
     */
    TSubclassOf<US1CameraMode> DetermineCameraMode() const;
    void InitializePlayerInput(UInputComponent* PlayerInputComponent);
    void Input_Move(const FInputActionValue& InputActionValue);
    void Input_LookMouse(const FInputActionValue& InputActionValue);


    /*
     *  member variables
     */
    UPROPERTY(EditAnywhere)
    TArray<FS1MappableConfigPair> DefaultInputConfig;
}; 
