
// Copyright (c) S1

#include "GameFeatureAction_WorldActionBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

UGameFeatureAction_WorldActionBase::UGameFeatureAction_WorldActionBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    // 월드를 순회하면서
    for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
    {
        // ExperienceManagerComponent에서 GameFeatureAction을 활성화하면서, Context에 World를 넣어줌
        //  - 이를 통해 적용할 대상인지 아닌지 판단
        if (Context.ShouldApplyToWorldContext(WorldContext))
        {
            // WorldActionBase의 Interace만 AddtoWorld 호출
            AddToWorld(WorldContext, Context);
        }
    }
}



