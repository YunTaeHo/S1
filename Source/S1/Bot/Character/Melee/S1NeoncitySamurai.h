
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "S1NeoncitySamurai.generated.h"

UENUM(BlueprintType)
enum class ENeoncitySamuraiState : uint8
{
    DefaultAttack,
    ThrowAttack,
    DashAttack,
    Dodge,

};

UCLASS()
class S1_API AS1NeoncitySamurai : public AS1BotCharacter
{
    GENERATED_BODY()

public:
    AS1NeoncitySamurai(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void DefaultAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ThrowAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void DashAttack(AActor* Target);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void Dodge(AActor* Target);


}; 
