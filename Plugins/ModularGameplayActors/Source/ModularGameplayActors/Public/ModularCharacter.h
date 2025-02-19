
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

    /** 죽음 관련 함수 */
protected:
    /** 죽음 이전에 호출할 이펙트, 시퀀스 위젯 를 처리한다 */
    UFUNCTION()
    virtual void OnDeathStarted(AActor* OwningActor) {};

    /** 죽음이 끝난 후 이펙트, 위젯, 맵 이동을 수행한다 */
    UFUNCTION()
    virtual void OnDeathFinished(AActor* OwningActor) {};

protected:
    UPROPERTY(BlueprintReadOnly, Category = "S1")
    bool bSuperArmor = false;

}; 
