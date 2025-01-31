
// Copyright (c) S1

#pragma once

#include "Containers/Array.h"
#include "Templates/SubclassOf.h"
#include "Inventory/S1InventoryItemDefinition.h"
#include "InventoryFragment_ReticleConfig.generated.h"

/** forward declarations */
class US1ReticleWidgetBase;

UCLASS()
class S1_API UInventoryFragment_ReticleConfig : public US1InventoryItemFragment
{
    GENERATED_BODY()

public:

    /** 무기에 결합된 ReticleWidget 정보를 가지고 있는 Fragment */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reticle")
    TArray<TSubclassOf<US1ReticleWidgetBase>> ReticleWidgets;

}; 
