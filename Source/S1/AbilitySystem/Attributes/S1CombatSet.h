
// Copyright (c) S1

#pragma once

#include "S1AttributeSet.h"
#include "S1CombatSet.generated.h"

UCLASS()
class S1_API US1CombatSet : public US1AttributeSet
{
    GENERATED_BODY()

public:
    US1CombatSet();

    ATTRIBUTE_ACCESSORS(US1CombatSet, BaseHeal);

    /*
     * FGameplayAttribute가 참고하는 실제 AttributeSet에 있는 데이터이다
     * - Healing의 단위를 의미한다
     * - e.g. 5.f라면, Period당 5씩 Healing 된다는 의미
     */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Combat")
    FGameplayAttributeData BaseHeal;

    /** 데미지도 BaseHeal과 마찬가지로 적용된다 */
    UPROPERTY(BlueprintReadOnly, Category = "S1|Combat")
    FGameplayAttributeData BaseDamage;


}; 
