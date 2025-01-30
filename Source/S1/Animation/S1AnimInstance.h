
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "S1AnimInstance.generated.h"

UCLASS()
class S1_API US1AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    /*
     *  UAnimInstance`s Interface
     */
    virtual void NativeInitializeAnimation() override;

    /*
     * memeber methdos
     */
    void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

    /** 해당 속성값은 Lyra의 AnimBP에서 사용되는 값이므로 정의해주자 */
    UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
    float GroundDistance = -1.f;

    /** GameplayTag와 AnimInstance의 속성값을 매핑해준다 */
    UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
    FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

}; 
