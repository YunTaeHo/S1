
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

/** Attack 관련 함수 */
public:
	virtual void CallOnAttackEnd() override;


/** Equip, Unequip 관련 변수 */
public:
	UFUNCTION(BlueprintCallable)
	virtual void UnequipWeapon(bool bEndPlayEquipped = false);

	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon(bool bBeginPlayEquipped = false);

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

	UPROPERTY(BlueprintReadOnly, Category = "S1|Equipment")
	bool bHasWeapon;


protected:
	UPROPERTY(BlueprintAssignable)
	FOnEquippedEnded OnEquippedEnded;

	UPROPERTY(BlueprintAssignable)
	FOnUnEquippedEnded OnUnEquippedEnded;
}; 
