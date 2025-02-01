
// Copyright (c) S1

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "S1AttributeSet.generated.h"

/*
 * 아래 매크로르 활용해 Attribute를 Setting한다
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/*
 * - S1에서 메인 Attribute Set Class이다
 */
UCLASS()
class S1_API US1AttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    US1AttributeSet();


}; 
