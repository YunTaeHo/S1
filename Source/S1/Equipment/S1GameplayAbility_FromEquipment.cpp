
// Copyright (c) S1

#include "S1GameplayAbility_FromEquipment.h"
#include "Equipment/S1EquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameplayAbility_FromEquipment)

US1GameplayAbility_FromEquipment::US1GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

US1EquipmentInstance* US1GameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
    // CurrentActorInfo의 AbilitySystemComponent와 CurrentSpecHandle을 활용하여, GameplayAbilitySpec을 가져옴
    // GiveAbility를 통해 CurrActorInfo와 CurrentSpecHandle이 같이 초기화된다
    if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
    {
        // GameplayAbility_FromEquipment은 EquipmentInstance로부터 GiveAbility를 진행했다
        // 즉, SourceObject에 EquipmentInstance가 저장되어 있다(EquipmentManager 확인)
        return Cast<US1EquipmentInstance>(Spec->SourceObject.Get());
    }

    return nullptr;
}

US1InventoryItemInstance* US1GameplayAbility_FromEquipment::GetAssociatedItem() const
{
    if (US1EquipmentInstance* Equipment = GetAssociatedEquipment())
    {
        // 아이템을 장착했다면 QuickBar에서 Instigator를 InventoryItemInstance로 설정한다
        // 그렇기 때문에 EquipmentInstance를 가져와서 Instigator를 캐스팅해주자
        return Cast<US1InventoryItemInstance>(Equipment->GetInstigator());
    }

    return nullptr;
}


