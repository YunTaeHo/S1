
// Copyright (c) S1

#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "GameUIPolicy.generated.h"

/** foward declarations */
class UPrimaryGameLayout;
class UCommonLocalPlayer;

USTRUCT()
struct FRootViewportLayoutInfo
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

    UPROPERTY(Transient)
    TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

    UPROPERTY(Transient)
    bool bAddedToViewport = false;

    bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }

    FRootViewportLayoutInfo() {}
    FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
        : LocalPlayer(InLocalPlayer)
        , RootLayout(InRootLayout)
        , bAddedToViewport(bIsInViewport)
    {}

};

/*
 * 블루프린트로 만들어 정책을 정해주도록 하자
 */
UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
    GENERATED_BODY()

public:
    void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
    void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
    void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);


    /*
     * member methdos 
     */
    void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
    void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

    TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);
    void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);

    /** LocalPlayer에 바인딩된 UI의 Layout */
    UPROPERTY(EditAnywhere)
    TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

    /** 보통 싱글 게임에서는 LocalPlayer-PrimaryGameLayout 있지만, 멀티 플레이인 경우 복수 개 가능(리플레이) */
    UPROPERTY(Transient)
    TArray<FRootViewportLayoutInfo> RootViewportLayouts;


}; 
