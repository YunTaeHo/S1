
// Copyright (c) S1

#pragma once

#include "S1InventoryItemDefinition.h"
#include "S1InventoryFragment_QuickBarIcon.generated.h"

UCLASS()
class S1_API US1InventoryFragment_QuickBarIcon : public US1InventoryItemFragment
{
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush AmmoBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FText DisplayNameWhenEquipped;
}; 
