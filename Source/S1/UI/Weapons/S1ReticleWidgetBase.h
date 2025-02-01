
// Copyright (c) S1

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "S1ReticleWidgetBase.generated.h"

/** foward declarations */
class US1WeaponInstance;
class US1InventoryItemInstance;

UCLASS()
class S1_API US1ReticleWidgetBase : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    US1ReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable)
    void InitializeFromWeapon(US1WeaponInstance* InWeapon);

    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponInitialized();

    /*
     * WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱
     */
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<US1WeaponInstance> WeaponInstance;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<US1InventoryItemInstance> InventoryInstance;


}; 
