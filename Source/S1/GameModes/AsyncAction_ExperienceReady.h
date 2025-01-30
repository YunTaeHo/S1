
// Copyright (c) S1

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Delegates/Delegate.h"
#include "AsyncAction_ExperienceReady.generated.h"

/** forward declaractions */
class US1ExperienceDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

UCLASS()
class S1_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출 */
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
    static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

    /*
     * UBlueprintAsyncActionBase Interface
     */
    virtual void Activate() override;

    /*
     * Step1 - Step4
     */
    void Step1_HandleGameStateSet(AGameStateBase* GameState);
    void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
    void Step3_HandleExperienceLoaded(const US1ExperienceDefinition* CurrentExperience);
    void Step4_BroadcastReady();

    /** BP에서 사용하는 Delegate */
    UPROPERTY(BlueprintAssignable)
    FExperienceReadyAsyncDelegate OnReady;

    /** WorldPtr를 단순 로직상 캐싱 */
    TWeakObjectPtr<UWorld> WorldPtr;



}; 
