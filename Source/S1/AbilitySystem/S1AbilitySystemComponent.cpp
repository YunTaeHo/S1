
// Copyright (c) S1

#include "S1AbilitySystemComponent.h"
#include "Abilities/S1GameplayAbility.h"
#include "Animation/S1AnimInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1AbilitySystemComponent)

US1AbilitySystemComponent::US1AbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
    FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
    check(ActorInfo);
    check(InOwnerActor);

    const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

    // Pawn이 바뀌면, 강제적으로 AnimInstance에 묶인 PropertyTag들을 재호출
    if (bHasNewPawnAvatar)
    {
        if (US1AnimInstance* S1AnimInst = Cast<US1AnimInstance>(ActorInfo->GetAnimInstance()))
        {
            S1AnimInst->InitializeWithAbilitySystem(this);
        }
    }

}

void US1AbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputPressed(Spec);

    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputPress ability task works.
    if (Spec.IsActive())
    {
        // Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    }
}

void US1AbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);

    // We don't support UGameplayAbility::bReplicateInputDirectly.
    // Use replicated events instead so that the WaitInputRelease ability task works.
    if (Spec.IsActive())
    {
        // Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
    }
}

void US1AbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (InputTag.IsValid())
    {
        // 허용된 GameplayAbility를 순회
        for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
        {
            // Ability가 존재하고, DynamicAbilityTags에 InputTag에 있을 경우, 
            // InputPressed/Held에 넣어 Ability 처리를 대기한다
            if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
            {
                InputPressedSepcHandles.AddUnique(AbilitySpec.Handle);
                InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
            }
        }
    }
}

void US1AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (InputTag.IsValid())
    {
        for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
        {
            if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
            {
                // Released 추가, Held 제거
                InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
                InputHeldSpecHandles.Remove(AbilitySpec.Handle);
            }
        }
    }
}

void US1AbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
    TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

    // InputHeldSpecHandles의 Ability 처리를 위해 AbilitiesToActivate를 추가한다
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
    {
        // ActivatableAbilties의 Handle 값 비교를 통해 GameplayAbilitySpec을 반환
        if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability && !AbilitySpec->IsActive())
            {
                const US1GameplayAbility* S1AbilityCDO = CastChecked<US1GameplayAbility>(AbilitySpec->Ability);

                // ActivationPolict가 WhileInputActive 속성이면 활성화로 등록
                if (S1AbilityCDO->ActivationPolicy == ES1AbilityActivationPolicy::WhileInputActive)
                {
                    AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
                }
            }
        }
    }

    for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSepcHandles)
    {
        if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = true;

                if (AbilitySpec->IsActive())
                {
                    // 이미 Ability가 활성화되어 있을 경우 Input Event(InputPressed)만 호출
                    AbilitySpecInputPressed(*AbilitySpec);
                }
                else
                {
                    const US1GameplayAbility* S1AbilityCDO = CastChecked<US1GameplayAbility>(AbilitySpec->Ability);

                    // ActivationPolicy가 OnInputTriggered 속성이면 활성화로 등록
                    if (S1AbilityCDO->ActivationPolicy == ES1AbilityActivationPolicy::OnInputTriggered)
                    {
                        AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
                    }
                }
            }
        }
    }

    // 등록된 AbilitiesToActivate를 한꺼번에 등록 시작
    for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
    {
        // 모든 것이 잘 진행되었다면, CallActivate 호출로 BP의 Activate 노드가 실행됨
        TryActivateAbility(AbilitySpecHandle);
    }

    // 이번 프레임에 Release되었다면, 관련 GameplayAbility 처리
    for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
    {
        if (FGameplayAbilitySpec* AbilitySepc = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySepc->Ability)
            {
                AbilitySepc->InputPressed = false;
                if (AbilitySepc->IsActive())
                {
                    AbilitySpecInputReleased(*AbilitySepc);

                }
            }
        }
    }

    // Held는 Released 될 때 제거됨
    InputPressedSepcHandles.Reset();
    InputReleasedSpecHandles.Reset();
}


