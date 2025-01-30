
// Copyright (c) S1

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "S1AbilitySet.generated.h"

/** forward declarations */
class US1GameplayAbility;
class US1AbilitySystemComponent;

USTRUCT(BlueprintType)
struct FS1AbilitySet_GameplayAbility
{
    GENERATED_BODY()

    /** 허용된 GameplayAbility */
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<US1GameplayAbility> Ability = nullptr;

    /** Input 처리를 위한 GameplayTag */
    UPROPERTY(EditDefaultsOnly)
    FGameplayTag InputTag;

    /** Ability의 허용 조건 (Level) */
    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;

};

USTRUCT(BlueprintType)
struct FS1AbilitySet_GrantedHandles
{
    GENERATED_BODY()

    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
    void TakeFromAbilitySystem(US1AbilitySystemComponent* S1ASC);

protected:
    /** 허용된 GameplayAbilitySpecHandle(int32) */
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/** Gameplay Ability를 좀 더 쉽게 관리하기 위한 Set */
UCLASS(BlueprintType)
class S1_API US1AbilitySet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    US1AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** ASC에 허용가능한 Ability를 추가한다 */
    void GiveToAbilitySystem(US1AbilitySystemComponent* S1ASC, FS1AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

    /** 허용된 GameplayAbilities */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
    TArray<FS1AbilitySet_GameplayAbility> GrantedGameplayAbilities;

}; 
