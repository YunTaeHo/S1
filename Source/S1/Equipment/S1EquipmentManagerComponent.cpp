
// Copyright (c) S1

#include "S1EquipmentManagerComponent.h"
#include "S1EquipmentDefinition.h"
#include "S1EquipmentInstance.h"
#include "AbilitySystemGlobals.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentManagerComponent)

US1EquipmentManagerComponent::US1EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EquipmentList(this)
{

}

US1AbilitySystemComponent* FS1EquipmentList::GetAbilitySystemComponent() const
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();

    // IAbilitySystemInterface를 OwningActor가 상속받고 있다면 ASC를 가져올 수 있도록 해주는 함수
    return Cast<US1AbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

US1EquipmentInstance* FS1EquipmentList::AddEntry(TSubclassOf<US1EquipmentDefinition> EquipmentDefinition)
{
    US1EquipmentInstance* Result = nullptr;
    check(EquipmentDefinition != nullptr);
    check(OwnerComponent);
    check(OwnerComponent->GetOwner()->HasAuthority());

    // EquipmentDefinition의 멤버 변수들은 EditDefaultOnly로 정의되어있어 CDO로 가져올 수 있다
    const US1EquipmentDefinition* EquipmentCDO = GetDefault<US1EquipmentDefinition>(EquipmentDefinition);

    TSubclassOf<US1EquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
    if (!InstanceType)
    {
        InstanceType = US1EquipmentInstance::StaticClass();
    }

    // Entries에 데이터 추가
    FS1AppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.EquipmentDefinition = EquipmentDefinition;
    NewEntry.Instance = NewObject<US1EquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
    Result = NewEntry.Instance;

    US1AbilitySystemComponent* ASC = GetAbilitySystemComponent();
    check(ASC);
    {
        for (TObjectPtr<US1AbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
        {
            AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
        }
    }

    // EquipmentInstance에서 Actor들을 인스턴스화 시켜준다
    Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

    return Result;
}



void FS1EquipmentList::RemoveEntry(US1EquipmentInstance* Instance)
{
    // Entries를 순회하며, Instance를 찾는다
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FS1AppliedEquipmentEntry& Entry = *EntryIt;
        if (Entry.Instance == Instance)
        {
            US1AbilitySystemComponent* ASC = GetAbilitySystemComponent();
            check(ASC);
            {
                // TakeFromAbilitySystem은 GiveToAbilitySystem 반대 역할로 ActivatableAbilties에서 제거한다
                Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
            }
            // Actor 제거 작업 및 Iterator를 통한 안전하게 Array에서 제거 진행
            Instance->DestroyEquipmentActors();
            EntryIt.RemoveCurrent();
        }
    }
}

US1EquipmentInstance* US1EquipmentManagerComponent::EquipItem(TSubclassOf<US1EquipmentDefinition> EquipmentDefinition)
{
    US1EquipmentInstance* Result = nullptr;

    if (EquipmentDefinition)
    {
        Result = EquipmentList.AddEntry(EquipmentDefinition);
        if (Result)
        {
            Result->OnEquipped();
        }
    }

    return Result;
}

void US1EquipmentManagerComponent::UnequipItem(US1EquipmentInstance* ItemInstance)
{
    if (ItemInstance)
    {
        // BP의 EventNode를 호출시켜준다
        ItemInstance->OnUnequipped();
    
        // EquipmentList에서 제거한다
        EquipmentList.RemoveEntry(ItemInstance);
    }
}

TArray<US1EquipmentInstance*> US1EquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<US1EquipmentInstance> InstanceType) const
{
    TArray<US1EquipmentInstance*> Results;

    // EquipmentList를 순회하며
    for (const FS1AppliedEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (US1EquipmentInstance* Instance = Entry.Instance)
        {
            // InstanceType에 맞는 Class면 Results에 추가하여 return
            if (Instance->IsA(InstanceType))
            {
                Results.Add(Instance);
            }
        }
    }

    return Results;
}

US1EquipmentInstance* US1EquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<US1EquipmentInstance> InstanceType)
{
    for (FS1AppliedEquipmentEntry& Entry : EquipmentList.Entries)
    {
        if (US1EquipmentInstance* Instance = Entry.Instance)
        {
            if (Instance->IsA(InstanceType))
            {
                return Instance;
            }
        }
    }

    return nullptr;
}


