
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

    /** Returns the S1 component if one exists on the specified actor. */
    UFUNCTION(BlueprintPure, Category = "S1|Hero")
    static US1PlayerComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<US1PlayerComponent>() : nullptr); }


    /** FeatureName 정의 */
    static const FName NAME_ActorFeatureName;
    static const FName NAME_BindInputsNow;

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
    void Input_Jump(const FInputActionValue& InputActionValue);
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);
        

    void SetAbilityCameraMode(TSubclassOf<US1CameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);
    void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
  
    /*
     *  member variables
     */
    UPROPERTY(EditAnywhere)
    TArray<FS1MappableConfigPair> DefaultInputConfig;

    /** Camera mode set by an ability. */
    UPROPERTY()
    TSubclassOf<US1CameraMode> AbilityCameraMode;

    /** Spec handle for the last ability to set a camera mode. */
    FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;

}; 
