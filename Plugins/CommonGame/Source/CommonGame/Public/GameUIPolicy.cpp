
// Copyright (c) S1

#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIPolicy)

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	// PC가 업데이트되면 GameLayout을 업데이트해주기 위해 Delegate를 추가해준다
	LocalPlayer->OnplayerControllerSet.AddWeakLambda(this, [this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	{
		// 추가된 Player가 있으면, 제거해주고
		NotifyPlayerRemoved(LocalPlayer);

		// RootViewportLayouts를 순회하며 검색
		if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
		{
			// Layout만 업데이트
			AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
			LayoutInfo->bAddedToViewport = true;
		}
		else
		{
			// Layout을 생성하고 활성화한다
			CreateLayoutWidget(LocalPlayer);
		}
	});

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);

		// 비활성화 확인용도
		LayoutInfo->bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	// Player가 Destroy되므로, OnPlayerControllerSet을 제거한다
	LocalPlayer->OnplayerControllerSet.RemoveAll(this);

	// RootViewportLayouts에 들어있는 Layout을 제거한다
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		// 만약 PrimaryGameLayout이 있다면, Viewport에서도 제거
		UPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);
		RemoveLayoutFromViewport(LocalPlayer, Layout);
	}
}

void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// CommonUserWidget의 PlayerContext를 설정
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));

	// 해당 Layout에 우선순위를 높게 설정(100)
	// - AddToPlayerScreen를 통해 Widget을 붙인다
	Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// UCommonUserWidget의 SlateWidget을 가져와 Parent와 연결고리를 끊는다
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	// PC가 할당되어있을 경우, LayoutWidget을 생성
	if (APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
	{
		// LayoutWidgetClass가 있을 경우 
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			// PC가 소유한다는 의미에서 Owner를 설정
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PC, LayoutWidgetClass);

			// FRootViewportLayoutInfo의 생성자를 호출
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}
