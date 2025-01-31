
// Copyright (c) S1

#include "S1ActivatableWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ActivatableWidget)

US1ActivatableWidget::US1ActivatableWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

TOptional<FUIInputConfig> US1ActivatableWidget::GetDesiredInputConfig() const
{
	// ES1WidgetInputMode에 따라 InputConfig를 설정해준다
	// - ECommonInputMode에 따라 Input의 흐름(All, Game, Menu) 설정이 가능하다
	//	- Meu로 들어갔을 경우, Game에 Input을 보낼 수 없도록 설정한다
	//	- MenuMode에서 Space를 눌렀을 경우 점프할 수 없도록 설정할 때 용이하다
	switch (InputConfig)
	{
	case ES1WidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);

	case ES1WidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);

	case ES1WidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);

	case ES1WidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}


