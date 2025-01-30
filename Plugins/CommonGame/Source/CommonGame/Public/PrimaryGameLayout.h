
// Copyright (c) S1

#pragma once

#include "CommonUserWidget.h"
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


}; 
