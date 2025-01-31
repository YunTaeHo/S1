
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "S1ActivatableWidget.generated.h"

/*
 * Input 처리 방식 정의
 */
UENUM(BlueprintType)
enum class ES1WidgetInputMode : uint8
{
    Default,
    GameAndMenu,
    Game,
    Menu,
};

/*
 * CommonActivatableWidget
 * 1. Widget Layout과 Instance를 구분하기 위한 용도로 사용
 *      - 런타임 Activate/Deactivate
 *      - WidgetTree에서 제거가 아닌 On/Off(==Activate/Deactivate)
 * 2. Input(Mouser or Keyboard etc..) 처리 방법 정의
 */
UCLASS(Abstract, Blueprintable)
class S1_API US1ActivatableWidget : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    US1ActivatableWidget(const FObjectInitializer& ObjectInitializer);

    /*
     * UCommonActivatableWidget`s Interface
     */
    virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

    /** Input 처리 방식 */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    ES1WidgetInputMode InputConfig = ES1WidgetInputMode::Default;

    /** Mouser 처리 방식 */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;


}; 
