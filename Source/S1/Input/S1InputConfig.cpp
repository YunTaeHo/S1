
// Copyright (c) S1

#include "S1InputConfig.h"
#include "S1LogChannel.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1InputConfig)

US1InputConfig::US1InputConfig(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

const UInputAction* US1InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    // NativeInputActions을 순회하며, Input으로 들어온 InputTag가 있는 지 체크
    for (const FS1InputAction& Action : NativeInputActions)
    {
        if (Action.InputAction && (Action.InputTag == InputTag))
        {
            return Action.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(LogS1, Error, TEXT("Can`t find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
    }

    return nullptr;
}

const UInputAction* US1InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
    return nullptr;
}


