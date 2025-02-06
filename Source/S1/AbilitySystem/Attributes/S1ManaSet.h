
// Copyright (c) S1

#pragma once

#include "S1AttributeSet.h"
#include "S1ManaSet.generated.h"

UCLASS(BlueprintType)
class S1_API US1ManaSet : public US1AttributeSet
{
    GENERATED_BODY()

public:
    US1ManaSet();

    /** 매크로로 Property, Init, Set, Ger을 정의한다 */
    ATTRIBUTE_ACCESSORS(US1ManaSet, Mana);
    ATTRIBUTE_ACCESSORS(US1ManaSet, MaxMana);
    ATTRIBUTE_ACCESSORS(US1ManaSet, AddMana);
    ATTRIBUTE_ACCESSORS(US1ManaSet, UseMana);

    /*
     * GameplayEffect가 HealSet에 Attribute를 수행하기 전에 불리는 콜백함수 :
     *  - Healing이 업데이트되면, Heatlh의 Healing을 적용하여 업데이트 가능
     */
    //virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
    //virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    ///*
    // * Attribute의 값을 ClampAttribute()를 활용해, 값의 범위를 유지시켜준다
    // * PreAttributeBaseChange와 PreAttributeChange를 오버라이드
    // */
    //void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
    //virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    UFUNCTION(BlueprintCallable)
    FGameplayAttributeData GetMana() { return Mana; }

    /** 현재 마나 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Mana")
    FGameplayAttributeData Mana;

    /** 마나 최대치 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Mana")
    FGameplayAttributeData MaxMana;

    /** 마나 회복치 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Mana")
    FGameplayAttributeData AddMana;

    /** 사용한 마나 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Mana")
    FGameplayAttributeData UseMana;

}; 
