
// Copyright (c) S1

#pragma once

#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    /*
     * Character`s Interfaces
     */
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    bool IsSuperArmored() const { return bSuperArmor; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "S1")
    bool bSuperArmor = false;

}; 
