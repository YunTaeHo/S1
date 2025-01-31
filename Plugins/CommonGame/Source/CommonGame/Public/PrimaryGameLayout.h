
// Copyright (c) S1

#pragma once

#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.generated.h"

/*
 * 인게임에서 메인 UI의 레이아웃음 담당하는 UMG
 * - 플레이어당 하나씩 가지는 최상위 UI 레이아웃
 * - 레이어 기반으로 UI를 관리
 */
UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** LayerName에 따른 ActivatableWidgetContainerBase를 가져옴 */
    UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}


    /** Layer를 추가하며, GameplayTag를 할당한다 */
    UFUNCTION(BlueprintCallable, Category = "Layer")
    void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);

    /** GameplayTag --- CommonActivatableWidgetContainerBase */
    UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
}; 
