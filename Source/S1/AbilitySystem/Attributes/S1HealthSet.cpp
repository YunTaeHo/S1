
// Copyright (c) S1

#include "S1HealthSet.h"
#include "GameplayEffectExtension.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1HealthSet)

// @TODO HORK 나중에 제대로 들어가면 다시 Heal값 세팅해주자
US1HealthSet::US1HealthSet()
    : Super()
    , Health(50.f)
    , MaxHealth(100.f)
{

}

bool US1HealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return Super::PreGameplayEffectExecute(Data);
}

void US1HealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    float MinimumHealth = 0.f;

    // Healing의 업데이트 될 경우 Healing을 Health에 적용하고 Healing을 초기화해준다
    if (Data.EvaluatedData.Attribute == GetHealingAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
        SetHealing(0.f);
    }
    // Health 업데이트 경우, [0, MaxHealth]로 맞춰준다
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
    }
}

void US1HealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
    // HealthAttribute는 [0, GetMaxHealth]로 설정
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }
    // MaxHealthAttribute는 [1.0, inf]로 설정
    // 1 미만은 될 수 없음
    else if (Attribute == GetMaxHealthAttribute())
    {
        NewValue = FMath::Max(NewValue, 1.0f);
    }

}

void US1HealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);
    ClampAttribute(Attribute, NewValue);
}

void US1HealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeBaseChange(Attribute, NewValue);
    ClampAttribute(Attribute, NewValue);
}


