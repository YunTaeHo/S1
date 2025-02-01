
// Copyright (c) S1

#pragma once

#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "S1HealthComponent.generated.h"

/** forward declarations */
class US1AbilitySystemComponent;
class US1HealthSet;
class US1HealthComponent;
class AActor;
struct FOnAttributeChangeData;

/** Health 변화 콜백을 위한 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FS1Health_AttributeChanged, US1HealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/*
 * Character(Pawn)에 대해 체력관련 처리를 담당하는 Component
 *  - 해당 클래스는 Blueprintable
 *  - 멤버변수인 Delegate를 UI에서 바인딩하기 위해 사용
 */
UCLASS(Blueprintable)
class S1_API US1HealthComponent : public UGameFrameworkComponent
{
    GENERATED_BODY()

public:
    US1HealthComponent(const FObjectInitializer& ObjectInitializer);

    /*
     * BP 지원 메서드
     */

    /** Actor(보통 ACharacter/APawn)의 HealthComponent를 반환 */
    UFUNCTION(BlueprintPure, Category = "S1|Health")
    static US1HealthComponent* FindHealthComponent(const AActor* Actor);

    /** 아래의 UFUNCTION은 HealthSet의 Attribute에 접근하기 위한 BP Accessor 함수들 */
    UFUNCTION(BlueprintPure, Category = "S1|Health")
    float GetHealth() const;

    UFUNCTION(BlueprintPure, Category = "S1|Health")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintPure, Category = "S1|Health")
    float GetHealthNormalized();

    /** ASC와 HealthSet 초기화 */
    void InitializeWithAbilitySystem(US1AbilitySystemComponent* InASC);
    void UnInitializeWithAbilitySystem();

    /** ASC를 통해, HealthSet의 HealthAttribute 변경이 있을 때 호출하는 메서드 (내부적으로 OnHealthChanged 호출) */
    void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

    /** HealSet을 접근하기 위해 AbilitySystemComponent */
    UPROPERTY()
    TObjectPtr<US1AbilitySystemComponent> AbilitySystemComponent;

    /** 캐싱된 HealSet 래퍼런스 */
    UPROPERTY()
    TObjectPtr<const US1HealthSet> HealthSet;

    /** health 변화에 따른 Delegate(Multicast) */
    UPROPERTY(BlueprintAssignable)
    FS1Health_AttributeChanged OnHealthChanged;

}; 
