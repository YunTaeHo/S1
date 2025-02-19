
// Copyright (c) S1

#pragma once

#include "S1InventoryItemDefinition.h"
#include "UObject/ObjectPtr.h"
#include "S1InventoryFragment_PickupIcon.generated.h"

class UObject;
class USkeletalMesh;

UCLASS()
class S1_API US1InventoryFragment_PickupIcon : public US1InventoryItemFragment
{
    GENERATED_BODY()

public:
    US1InventoryFragment_PickupIcon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FLinearColor PadColor;
}; 
