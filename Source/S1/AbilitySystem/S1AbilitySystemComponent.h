
// Copyright (c) S1

#pragma once

#include "AbilitySystemComponent.h"
#include "S1AbilitySystemComponent.generated.h"



UCLASS()
class S1_API US1AbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    US1AbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UAbilitySystemComponent`s Interface
     */
    virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

    /*
     * memeber methods
     */
    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag&  InputTag);
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);


    /** Ability Input 처리할 Pending Queue */
    TArray<FGameplayAbilitySpecHandle> InputPressedSepcHandles;
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
}; 
