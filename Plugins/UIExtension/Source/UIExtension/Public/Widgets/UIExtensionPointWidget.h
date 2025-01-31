
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

/** foward dclarations */
class UCommonLocalPlayer;

/*
 * UUIExtension에 붙을 위젯 하나의 단위
 *  - UI Extension에 결합된 Context별로 관리하는 객체가 UIExtensionPoint(Handle)이다
 *  - 해당 UIExtension Slot에 PlayerState/PlayerController/LocalPlayer와 같이 다양한 Context로부터 Widget이 결합될 수 있다
 *     (같은 위젯이 붙어도 서로 Context가 다르기 때문에 다른 위젯이 된다)
 * 
 *    이를 분리하여 관리하기 위해 만들어진 객체가 UIExtensionPoint이다
 */
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
    GENERATED_BODY()

public:
    UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void ResetExtensionPoint();
    void RegisterExtensionPoint();
    void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);
    void OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request);

    /*
     * UWidget`s Interfaces
     */
    virtual TSharedRef<SWidget> RebuildWidget() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    FGameplayTag ExtensionPointTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

    /** UIExtensionPointWidget에 결합이 허용된 UWidget Classes */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    TArray<TObjectPtr<UClass>> DataClasses;

    /** UIExtension --- Widget 관계 매핑*/
    UPROPERTY(Transient)
    TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

    /** UIExtensionPoint 객체 관리*/
    TArray<FUIExtensionPointHandle> ExtensionPointHandles;


}; 
