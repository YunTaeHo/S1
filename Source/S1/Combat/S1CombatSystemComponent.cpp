
// Copyright (c) S1

#include "S1CombatSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1CombatSystemComponent)

US1CombatSystemComponent::US1CombatSystemComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

bool US1CombatSystemComponent::ReserveAttackToken(int32 Amount)
{
    if (AttackTokenCount >= Amount)
    {
        AttackTokenCount -= Amount;
        return true;
    }

    return false;
}

void US1CombatSystemComponent::ReturnAttackToken(int32 Amount)
{
    AttackTokenCount += Amount;
}


