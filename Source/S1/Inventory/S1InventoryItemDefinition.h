
// Copyright (c) S1

#pragma once

#include "UObject/Object.h"
#include "S1InventoryItemDefinition.generated.h"

/*
 *  
 *  Component의 개념과 비슷하다고 볼 수 있다
 *   ex ) 
 *        InventoryFragement_EquippableItem은 EquipmentItemDefinition을 가지고 있어 장착 가능한 아이템에서 사용한다
          InventoryFragment_SetStats는 아이템에 대한 정보를 가지고 있다
 *         - Rifle에 대한 SetStats으로 총알(Ammo)에 대한 장착 최대치와 현재 남은 탄창 수를 예시로 들 수 있다
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class S1_API US1InventoryItemFragment : public UObject
{
    GENERATED_BODY()
};


UCLASS(BlueprintType, Blueprintable)
class S1_API US1InventoryItemDefinition : public UObject
{
    GENERATED_BODY()

public:
    US1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    const US1InventoryItemFragment* FindFragmentByClass(TSubclassOf<US1InventoryItemFragment> FragmentClass) const;

    /** Inventory Item 정의(메타) 이름*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    FText DisplayName;

    /** Inventory Item의 Component를 Fragment로 인식하면 된다 */
    UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Display")
    TArray<TObjectPtr<US1InventoryItemFragment>> Fragments;

}; 
