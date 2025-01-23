
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "S1ExperienceManagerComponent.generated.h"

class US1ExperienceDefinition;

enum class ES1ExperienceLoadState
{
    Unloaded,
    Loading,
    Loaded,
    Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnS1ExperienceLoaded, const US1ExperienceDefinition*);

/*
 *  S1ExperienceManagerComponent
 *  - 말 그대로, 해당 Component는 GameState를 Owner로 가지면서, Experience의 상태 정보를 가지고 있는 Component
 *  - Experience 로딩 상태 업데이트 및 이벤트를 관리한다
 */
UCLASS()
class S1_API US1ExperienceManagerComponent : public UGameStateComponent
{
    GENERATED_BODY()

public:
    US1ExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     *  member methods
     */
    bool IsExperienceLoaded() { return (LoadState == ES1ExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

    /*
     *  아래의 OnExperienceLoaded에 바인딩허거나, 이미 Experience 로딩이 완료되었다면 바로 호춣함
     */
    void CallorRegister_OnExperienceLoaded(FOnS1ExperienceLoaded::FDelegate&& Delegate);

    void SetCurrentExperience(FPrimaryAssetId ExperienceId);
    void StartExperienceLoad();
    void OnExperienceLoadComplete();
    void OnExperienceFullLoadComplete();
    const US1ExperienceDefinition* GetCurrentExperienceChecked() const;

    /*
     *  member variables
     */

    UPROPERTY()
    TObjectPtr<const US1ExperienceDefinition> CurrentExperience;

    /** Experience의 로딩 상태 모니터링 */
    ES1ExperienceLoadState LoadState = ES1ExperienceLoadState::Unloaded;

    /** Experience 로딩이 완료된 이후, Broadcasting Delegate */
    FOnS1ExperienceLoaded OnExperienceLoaded;

}; 
