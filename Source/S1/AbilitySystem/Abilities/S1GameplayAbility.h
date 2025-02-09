
// Copyright (c) S1

#pragma once

#include "Abilities/GameplayAbility.h"
#include "S1GameplayAbility.generated.h"

/** foward declarations */
class US1AbilityCost;
class US1CameraMode;
class AS1Character;
class AS1PlayerController;
class US1PlayerComponent;

UENUM(BlueprintType)
enum class ES1AbilityActivationPolicy : uint8
{
    /** Input이 Trigger 되었을 경우(Pressed/Released) */
    OnInputTriggered,
    /** Input이 Held되어 있을 경우 */
    WhileInputActive,
    /** avatar가 생성되었을 경우 바로 할당*/
    OnSpawn,
};

UCLASS()
class S1_API US1GameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    US1GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    /*
     * UGameplayAbility`s Interface 
     */
    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
    //virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;

    void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
    US1PlayerComponent* GetS1CharacterFromActorInfo() const;

    /** 해당 어빌리티의 카메라 모드로 세팅 */
    UFUNCTION(BlueprintCallable, Category = "S1|Ability")
    void SetCameraMode(TSubclassOf<US1CameraMode> CameraMode);

    /** 카메라 모드를 Clear 시킨다. 어빌리티가 끝날 때 자동으로 호출된다*/ 
    UFUNCTION(BlueprintCallable, Category = "S1|Ability")
    void ClearCameraMode();

    /** GA 활성화 정책(언제 활성화 될지?) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|AbilityActivation")
    ES1AbilityActivationPolicy ActivationPolicy;

    /** GameplayAbility가 Ability Cost를 사용해 Cost 관리 할 수 있도록 배열로 설정 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|AbilityActivation")
    TArray<TObjectPtr<US1AbilityCost>> AdditionalCosts;

    /** 현재 카메라 모드를 어빌리티에 세팅해준다*/
    TSubclassOf<US1CameraMode> ActiveCameraMode;

}; 
