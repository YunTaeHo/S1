
// Copyright (c) S1

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

/** foward declarations */
class UGameUIPolicy;
class UCommonLocalPlayer;

/*
 * GameUIManagerSubsystem은 GameInstanceSubsystem의 기반한다
 * - UGameUIManagerSubsystemdms UCLASS 속성으로 Abstract가 정의되어 있다
 */
UCLASS(Abstract, config=Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UGameUIManagerSubsystem();

    /*
     * UGameInstanceSubsystem`s Interfaces
     */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    /*
     * memeber methods
     */
    void SwitchToPolicy(UGameUIPolicy* InPolicy);
    void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
    void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
    void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

    UPROPERTY(Transient)
    TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

    /*
     * Default UI Policy를 생성할 class
     *  -    해당 클래스는 B_S1GameUIPolicy로 지정
     */
    UPROPERTY(Config, EditAnywhere)
    TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;

}; 
