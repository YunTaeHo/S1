
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "Input/S1MappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

/** 전방 선언 */
struct FComponentRequestHandle;
class APawn;

UCLASS()
class S1_API UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    UGameFeatureAction_AddInputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UGameFeatureAction`s Interface
     */
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

    /*
     * UGameFeatureAction_WorldActionBase Interface
     */
    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
    struct FPerContextData
    {
        TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
        TArray<TWeakObjectPtr<APawn>> PawnAddedTo;
    };

    /** GameFeatureState가 변경될 때, 콜백 함수 (혹은 ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능) */
    void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    /** InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거 */
    void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
    void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

    /** ActivateData를 초기화 */
    void Reset(FPerContextData& ActiveData);

    /** world -> (extension request, pawn )*/
    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    /** PlayerMappableInputConfig를 들고 있는 멤버 변수 */
    UPROPERTY(EditAnywhere)
    TArray<FS1MappableConfigPair> InputConfigs;

}; 
