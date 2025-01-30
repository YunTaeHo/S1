
// Copyright (c) S1

#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "S1GameFeaturePolicy.generated.h"

/*
 * US1GameFeaturePolicy는 GameFeature Plugin이 Memory에 로딩되거나 Active(활성화)를 관리하는 
 * StateMachine을 모니터링 가능하다
 */
UCLASS()
class S1_API US1GameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
    GENERATED_BODY()

public:
    US1GameFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UDefaultGameFeaturesProjectPolicies Interfaces
     */
    virtual void InitGameFeatureManager() override;
    virtual void ShutdownGameFeatureManager() override;

    /** Observer로서 등록한 객체를 관리(ex : US1GameFeature_AddGameplayCuePaths)*/
    UPROPERTY(Transient)
    TArray<TObjectPtr<UObject>> Observers;
}; 

/*
 * GameFeature Plugin이 Register/Unregister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 관련 GameplayCue 에셋을 스캔할 수 있도록 해야한다
 * 이를 위해 GameFeatureStateChangeObserver를 활용해, 옵저버 패턴으로 이를 가능하게 하도록 한다 : 
 *  - IGameFeatureStateChangeObserver를 활용해 옵저버 패턴 활용
 *  - 추가적 구현은 US1GameFeaturePolicy 참고
 * Registering/Unreigstering를 직접 호출해서 관리해보도록 하자
 * 
 */
UCLASS()
class US1GameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
    GENERATED_BODY()
public:
    /*
     *  IGameFeatureStateChangeObserver` s Interface
     */
    virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
    virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;

};
