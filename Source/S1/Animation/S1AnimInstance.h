
// Copyright (c) S1

#pragma once

#include "Animation/AnimInstance.h"
#include "S1AnimInstance.generated.h"

UCLASS()
class S1_API US1AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    /** 해당 속성값은 Lyra의 AnimBP에서 사용되는 값이므로 정의해주자 */
    UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
    float GroundDistance = -1.f;

}; 
