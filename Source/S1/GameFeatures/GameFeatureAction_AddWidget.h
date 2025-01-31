
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddWidget.generated.h"

/** foward declarations */
struct FComponentRequestHandle;

/** HUD의 Layout 요청 */
USTRUCT()
struct FS1HUDLayoutRequest
{
    GENERATED_BODY()

    /** UI의 레이아웃으로 CommonActivatableWidget을 사용 */
    UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles="Client"))
    TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

    /** 앞서 보았던 PrimaryGameLayout의 LayerID를 의미 */
    UPROPERTY(EditAnywhere, Category = UI)
    FGameplayTag LayerID;
};


USTRUCT()
struct FS1HUDElementEntry
{
    GENERATED_BODY()

    /** S1HUDLayout 위에 올릴 대상이 되는 Widget Class */
    UPROPERTY(EditAnywhere, Category = "UI", meta = (AssetBundles = "Client"))
    TSoftClassPtr<UUserWidget> WidgetClass;

    /** SlotID는 S1HUDLayoutRequest에 올린 LayoutClass에 정의된 Slot(GameplayTag)를 의미 */
    UPROPERTY(EditAnywhere, Category = "UI")
    FGameplayTag SlotID;
};

UCLASS(meta = (DisplayName = "Add Widgets"))
class S1_API UGameFeatureAction_AddWidget final : public UGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    struct FPerContextData
    {
        TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
        TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

        /** Lyra에서 HUDElement는 UIExtension으로 관리된다 */
        TArray<FUIExtensionHandle> ExtensionHandles;
    };

    void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
    void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

    void Reset(FPerContextData& ActiveData);
    void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    /*
     * UGameFeatureAction`s Interface
     */
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

    /*
     * UGameFeatureAction_WorldActionBase`s Interface
     */
    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

    /** GFA Add/Remove 상태 관리 */
    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    /*
     * GFA_AddWidget은 형태를 정의하는 Layout과 Layout 위에 올릴 Widget 객체로 Widgets를 구성한다
     */
    UPROPERTY(EditAnywhere, Category = UI)
    TArray<FS1HUDLayoutRequest> Layout;

    UPROPERTY(EditAnywhere, Category = UI)
    TArray<FS1HUDElementEntry> Widgets;


}; 
