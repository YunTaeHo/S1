
// Copyright (c) S1

#include "S1HealthComponent.h"
#include "S1LogChannel.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/S1HealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1HealthComponent)

US1HealthComponent::US1HealthComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // HealthComponent는 PlayerState의 HealthSet과 Character(Pawn)간 Bridge 역할이다
    // - 즉, 로직을 업데이트 할 필요가 없기 때문에 Tick을 돌리지 않는다
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;

    // InitializeWithAbilitySystem으로 ASC가 초기화되기 전까지 HealthSet과 ASC는 nullptr이다
    AbilitySystemComponent = nullptr;
    HealthSet = nullptr;
}

US1HealthComponent* US1HealthComponent::FindHealthComponent(const AActor* Actor)
{
    if (!Actor)
    {
        return nullptr;
    }

    US1HealthComponent* HealthComponent = Actor->FindComponentByClass<US1HealthComponent>();
    return HealthComponent;
}

float US1HealthComponent::GetHealth() const
{
    return (HealthSet ? HealthSet->GetHealth() : 0);
}

float US1HealthComponent::GetMaxHealth() const
{
    return (HealthSet ? HealthSet->GetMaxHealth() : 0);
}

float US1HealthComponent::GetHealthNormalized()
{
    if (HealthSet)
    {
        const float Health = HealthSet->GetHealth();
        const float MaxHealth = HealthSet->GetMaxHealth();
        return ((MaxHealth > 0.f) ? (Health / MaxHealth) : 0.f);
    }

    return 0.0f;
}

void US1HealthComponent::InitializeWithAbilitySystem(US1AbilitySystemComponent* InASC)
{
    AActor* Owner = GetOwner();
    check(Owner);

    if (AbilitySystemComponent)
    {
        UE_LOG(LogS1, Error, TEXT("S1HealthComponent : Health component for owner [%s] has already been initialized with an ability system"), *GetNameSafe(Owner));
        return;
    }

    AbilitySystemComponent = InASC;
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogS1, Error, TEXT("S1HealthComponent : Cannot initialize health component for owner [%s] with ability system"), *GetNameSafe(Owner));
        return;
    }

    // AbilitySystemComponent::GetSet은 SpawnedAttributes에서 가져온다
    HealthSet = AbilitySystemComponent->GetSet<US1HealthSet>();
    if (!HealthSet)
    {
        UE_LOG(LogS1, Error, TEXT("S1HealthComponent : Cannot initialize health component for owner [%s] with NULL heatlh set on the ability system"), *GetNameSafe(Owner));
        return;
    }

    // HealthSet의 HealthAttribute가 업데이트 될 때마다 호출할 콜백으로 멤버메서드 HandleHealthChanged를 등록하자
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(US1HealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

    // 초기화 한번 해줬으니 Broadcast 호출
    OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
}

void US1HealthComponent::UnInitializeWithAbilitySystem()
{
    AbilitySystemComponent = nullptr;
    HealthSet = nullptr;
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
    // GameEffectModifier에 Data가 있을 경우만 호출된다
    if (ChangeData.GEModData)
    {
        const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
        return EffectContext.GetOriginalInstigator();
    }
    return nullptr;
}


void US1HealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}


