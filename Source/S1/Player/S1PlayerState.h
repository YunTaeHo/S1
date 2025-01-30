
// Copyright (c) S1

#pragma once

#include "GameFramework/PlayerState.h"
#include "S1PlayerState.generated.h"

/** forward declaration*/
class US1PawnData;
class US1ExperienceDefinition;
class US1AbilitySystemComponent;

UCLASS()
class S1_API AS1PlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AS1PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void PostInitializeComponents() final;

    /*
     *  member methods
     */
    template<typename T>
    const T* GetPawnData() const { return Cast<T>(PawnData); }
    void OnExperienceLoaded(const US1ExperienceDefinition* CurrentExperience);
    void SetPawnData(const US1PawnData* InPawnData);
    US1AbilitySystemComponent* GetS1AbilitySystemComponent() const { return AbilitySystemComponent; }

    UPROPERTY()
    TObjectPtr<const US1PawnData> PawnData;

    UPROPERTY(VisibleAnywhere, Category = "S1|PlayerState")
    TObjectPtr<US1AbilitySystemComponent> AbilitySystemComponent;
}; 
