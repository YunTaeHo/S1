
// Copyright (c) S1

#include "S1EquipmentDefinition.h"
#include "S1EquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentDefinition)

US1EquipmentDefinition::US1EquipmentDefinition(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 기본값으로 설정(이렇게 들어오면 에러라고 생각하고 디버깅하자)
    InstanceType = US1EquipmentInstance::StaticClass();
}


