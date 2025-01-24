
// Copyright (c) S1

#pragma once

#include "Containers/Array.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "S1InputConfig.generated.h"

/** 전방 선언 */
class UInputAction;

/** S1InputAction은 GmaeplayTag와 InputAction을 연결하는 래퍼 클래스이다 */
USTRUCT(BlueprintType)
struct FS1InputAction
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UInputAction> InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputConfig"))
    FGameplayTag InputTag;

};

UCLASS(BlueprintType)
class S1_API US1InputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    US1InputConfig(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

    /*
     *  member variables
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
    TArray<FS1InputAction> NativeInputActions;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
    TArray<FS1InputAction> AbilityInputActions;

}; 
