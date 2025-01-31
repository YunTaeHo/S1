
// Copyright (c) S1

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "S1WeaponUserInterface.generated.h"

/** foward declarations */
class US1WeaponInstance;

/*
 * 무기 전용 UI
 */
UCLASS()
class S1_API US1WeaponUserInterface : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    US1WeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Weapon 변경에 따른 BP Event*/
    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponChanged(US1WeaponInstance* OldWeapon, US1WeaponInstance* NewWeapon);

    // @TODO HORK Delegate 방식으로 진행하는 것이 좋아보임
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    /** 현재 장착된 WeaponInstance를 추적한다 (NativeTick을 활용하여 주기적 업데이트) */
    UPROPERTY(Transient)
    TObjectPtr<US1WeaponInstance> CurrentInstance;


}; 
