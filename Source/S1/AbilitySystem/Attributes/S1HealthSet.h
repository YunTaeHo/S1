
// Copyright (c) S1

#pragma once

#include "S1AttributeSet.h"
#include "S1HealthSet.generated.h"

/*
 * HealthSet은 의미 그대로, 체력에 대한 속성값을 관리한다
 */
UCLASS(BlueprintType)
class S1_API US1HealthSet : public US1AttributeSet
{
    GENERATED_BODY()

public:
    US1HealthSet();

    /** 매크로로 Property, Init, Set, Ger을 정의한다 */
    ATTRIBUTE_ACCESSORS(US1HealthSet, Health);
    ATTRIBUTE_ACCESSORS(US1HealthSet, MaxHealth);
    ATTRIBUTE_ACCESSORS(US1HealthSet, Healing);

    /*
     * GameplayEffect가 HealSet에 Attribute를 수행하기 전에 불리는 콜백함수 :
     *  - Healing이 업데이트되면, Heatlh의 Healing을 적용하여 업데이트 가능
     */
    virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /*
     * Attribute의 값을 ClampAttribute()를 활용해, 값의 범위를 유지시켜준다
     * PreAttributeBaseChange와 PreAttributeChange를 오버라이드 
     */
    void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    /** 현재 체력 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Health")
    FGameplayAttributeData Health;

    /** 체력 최대치 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Health")
    FGameplayAttributeData MaxHealth;

    /** 체력 회복치 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Health")
    FGameplayAttributeData Healing;



}; 
