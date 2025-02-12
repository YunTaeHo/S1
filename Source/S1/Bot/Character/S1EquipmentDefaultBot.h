
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "S1EquipmentDefaultBot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquippedEnded);

/** foward declarations */
class UWidgetComponent;
class UClientUserWidget;

UCLASS()
class S1_API AS1EquipmentDefaultBot : public AS1BotCharacter
{
    GENERATED_BODY()

public:
    AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetMovementSpeed(EMovementState MoveState) override;

/** Attack 관련 함수 */
public:
	virtual void CallOnAttackEnd() override;


/** Equip, Unequip 관련 변수 */
public:
	UFUNCTION(BlueprintCallable)
	virtual void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void CallOnEquippedEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void CallOnUnequippedEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void NotfiyOnEquippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void NotfiyOnUnequippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);


private:
	void RemoveDelegates();


protected:
	// 이 둘은 나중에 보스랑 비교해야해서 따로 뺄 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "S1|Character")
	TObjectPtr<UWidgetComponent> Widget;

	UPROPERTY(EditDefaultsOnly, Category = "S1|Widget")
	TSubclassOf<UClientUserWidget> HeathBarWidget;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<UAnimMontage> EquippedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Equipment")
	float EquippedMontagePlayRate = 1.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<UAnimMontage> UnequippedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Equipment")
	float UnequippedMontagePlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Equipment")
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<AActor> Weapon;



protected:
	/** Attack Montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Attack Montage Rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "S1|Attack")
	float AttackMontagePlayRate = 1.f;

	UPROPERTY(BlueprintAssignable)
	FOnEquippedEnded OnEquippedEnded;

	UPROPERTY(BlueprintAssignable)
	FOnUnEquippedEnded OnUnEquippedEnded;
}; 
