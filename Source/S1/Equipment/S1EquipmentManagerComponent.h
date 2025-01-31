
// Copyright (c) S1

#pragma once

#include "Components/PawnComponent.h"
#include "AbilitySystem/S1AbilitySet.h"
#include "S1EquipmentManagerComponent.generated.h"

/** forward declarations */
class US1EquipmentDefinition;
class US1EquipmentInstance;


USTRUCT(BlueprintType)
struct FS1AppliedEquipmentEntry
{
    GENERATED_BODY()

    /** 장착물에 대한 메타 데이터 */
    UPROPERTY()
    TSubclassOf<US1EquipmentDefinition> EquipmentDefinition;

    /** EquipmentDefinition을 통해 생성된 인스턴스 */
    UPROPERTY()
    TObjectPtr<US1EquipmentInstance> Instance = nullptr;

    /** 무기에 할당된 허용가능한 GameplayAbility */
    UPROPERTY()
    FS1AbilitySet_GrantedHandles GrantedHandles;
};


USTRUCT(BlueprintType)
struct FS1EquipmentList
{
    GENERATED_BODY()

    FS1EquipmentList(UActorComponent* InOwnerComponent = nullptr)
        : OwnerComponent(InOwnerComponent)
    {}

    US1AbilitySystemComponent* GetAbilitySystemComponent() const;

    US1EquipmentInstance* AddEntry(TSubclassOf<US1EquipmentDefinition> EquipmentDefinition);
    void RemoveEntry(US1EquipmentInstance* Instance);

    /** 장착물에 대한 관리 리스트 */
    UPROPERTY()
    TArray<FS1AppliedEquipmentEntry> Entries;

    UPROPERTY()
    TObjectPtr<UActorComponent> OwnerComponent;
};

/*
 * Pawn의 Component로서 장착물에 대한 관리를 담당한다
 */
UCLASS()
class S1_API US1EquipmentManagerComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    US1EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    US1EquipmentInstance* EquipItem(TSubclassOf<US1EquipmentDefinition> EquipmentDefinition);
    void UnequipItem(US1EquipmentInstance* ItemInstance);

    UFUNCTION(BlueprintCallable)
    TArray<US1EquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<US1EquipmentInstance> InstanceType) const;

    /** 장착물 중 처음 것을 반환 없으면 NULL */
    US1EquipmentInstance* GetFirstInstanceOfType(TSubclassOf<US1EquipmentInstance> InstanceType);

    template<typename T>
    T* GetFirstInstanceOfType()
    {
        return (T*)GetFirstInstanceOfType(T::StaticClass());
    }


    UPROPERTY()
    FS1EquipmentList EquipmentList;
}; 
