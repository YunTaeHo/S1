
// Copyright (c) S1

#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "S1InputConfig.h"
#include "S1InputComponent.generated.h"

UCLASS()
class S1_API US1InputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

public:
    US1InputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     *  member methods
     */
    template <class UserClass, typename FuncType>
    void BindNativeActions(const US1InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

    template <class UserClass, class PressedFuncType, typename ReleasedFuncType>
    void BindAbilityActions(const US1InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);


}; 

template<class UserClass, typename FuncType>
void US1InputComponent::BindNativeActions(const US1InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
    check(InputConfig);
    
    // InputAction을 찾아와 EnhancedInput에 Bind 해준다
    if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
    {
        BindAction(IA, TriggerEvent, Object, Func);
    }
    
}

template<class UserClass, class PressedFuncType, typename ReleasedFuncType>
void US1InputComponent::BindAbilityActions(const US1InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
    check(InputConfig);

    // AbilityAction에 대해서는 모든 InputAction에 다 바인딩 시킨다
    for (const FS1InputAction& Action : InputConfig->AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag.IsValid())
        {
            if (PressedFunc)
            {
                BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
            }

            if (ReleasedFunc)
            {
                BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
            }
        }
    }
}
