
// Copyright (c) S1

#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "System/S1GameplayTagStack.h"
#include "S1InventoryItemInstance.generated.h"

/** forward declarations */
class US1InventoryItemDefinition;
class US1InventoryItemFragment;

/*
 * 해당 클래스는 Inventory Item의 인스턴스로 볼 수 있다
 */
UCLASS(BlueprintType)
class S1_API US1InventoryItemInstance : public UObject
{
    GENERATED_BODY()

public:
    US1InventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = "FragmentClass"))
    const US1InventoryItemFragment* FindFragmentByClass(TSubclassOf<US1InventoryItemFragment> FragmentClass) const;

    template<typename ResultClass>
    const ResultClass* FindFragmentByClass() const
    {
        return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
    }

    /** Stat에서 Tag와 Count를 통해 생성/삭제 시켜준다 */
    void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
    void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

    /** Stat에서 해당 태그를 가지고 있는 지 확인한다 */
    bool HasStatTag(FGameplayTag Tag) const;

    /** 현재 Tag를 stat안에 Tag에 관련된 Count가 몇 개 있는지 확인한다 */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetStatTagStackCount(FGameplayTag Tag) const;

    /** ItemIstance용 GameplayTagStack */
    FS1GameplayTagStackContainer StatTags;


    /** Inventroy Imte의 인스턴스에는 무엇으로 정의되었는지 메타 클래스인 US1InventoryItemDefinition를 들고 있다*/
    UPROPERTY()
    TSubclassOf<US1InventoryItemDefinition> ItemDef;
}; 
