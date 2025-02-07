
// Copyright (c) S1

#pragma once

#include "Character/S1BotCharacter.h"
#include "S1EquipmentDefaultBot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquippedEnded);

UCLASS()
class S1_API AS1EquipmentDefaultBot : public AS1BotCharacter
{
    GENERATED_BODY()

public:
    AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void SetMovementSpeed() override;
	

/** Attack 관련 함수 */
public:
	UFUNCTION(BlueprintCallable)
	void CallOnAttackEnd();


/** Equip, Unequip 관련 변수 */
public:
	UFUNCTION(BlueprintCallable)
	virtual void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void CallOnEquippedEnded();

	UFUNCTION(BlueprintCallable)
	void CallOnUnequippedEnded();

/** Equip, Unequip 관련 변수 */
protected:
	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<UAnimMontage> EquippedMontage;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	float EquippedMontagePlayRate = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<AActor> Weapon;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	TObjectPtr<UAnimMontage> UnequippedMontage;

	UPROPERTY(BlueprintReadWrite, Category = "S1|Equipment")
	float UnequippedMontagePlayRate = 1.f;


/** Attack 관련 변수 */
protected:
	/** Attack Montage*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Attack Montage Rate*/
	UPROPERTY(BlueprintReadWrite, Category = "S1|Attack")
	float AttackMontagePlayRate = 1.f;

	UPROPERTY(BlueprintAssignable)
	FOnEquippedEnded OnEquippedEnded;

	UPROPERTY(BlueprintAssignable)
	FOnUnEquippedEnded OnUnEquippedEnded;
}; 
