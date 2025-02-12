
// Copyright (c) S1

#include "S1RangedDefaultBot_LineTrace.h"
#include "Bot/S1BotCombatSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1RangedDefaultBot_LineTrace)

AS1RangedDefaultBot_LineTrace::AS1RangedDefaultBot_LineTrace(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1RangedDefaultBot_LineTrace::Attack(AActor* Target)
{
    BotCombatSystemComponent->SetAttack(true);
    PlayAttackMontage(Target);
}

void AS1RangedDefaultBot_LineTrace::EndAttack(AActor* AttackTarget, int32 TokensNeeded)
{
    Super::EndAttack(AttackTarget, TokensNeeded);
    BotCombatSystemComponent->SetAttack(false);
}


