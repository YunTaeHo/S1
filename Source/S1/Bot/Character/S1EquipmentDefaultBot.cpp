
// Copyright (c) S1

#include "S1EquipmentDefaultBot.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentDefaultBot)

AS1EquipmentDefaultBot::AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1EquipmentDefaultBot::BeginPlay()
{
    Super::BeginPlay();
}

void AS1EquipmentDefaultBot::CallOnAttackEnd()
{
    OnAttackEnded.Broadcast();
    RemoveDelegates();
}

void AS1EquipmentDefaultBot::CallOnEquippedEnded(UAnimMontage* Montage, bool bInterrupted)
{
    OnEquippedEnded.Broadcast();

    if (Montage)
    {
        RemoveDelegates();
    }
}

void AS1EquipmentDefaultBot::CallOnUnequippedEnded(UAnimMontage* Montage, bool bInterrupted)
{
    OnUnEquippedEnded.Broadcast();
    if (Montage)
    {
        RemoveDelegates();
    }
}

void AS1EquipmentDefaultBot::NotfiyOnEquippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == TEXT("Equipped"))
    {
        Weapon = GetWorld()->SpawnActor<AActor>(WeaponClass);
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r"));
        bHasWeapon = true;
    }
    
}

void AS1EquipmentDefaultBot::NotfiyOnUnequippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == TEXT("Unequipped"))
    {
        if (Weapon)
        {
            Weapon->Destroy();
            bHasWeapon = false;
        }
    }
}

void AS1EquipmentDefaultBot::UnequipWeapon(bool bEndPlayEquipped)
{
    if (bEndPlayEquipped)
    {
        if (Weapon)
        {
            Weapon->Destroy();
            GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]() { OnUnEquippedEnded.Broadcast();}));
            bHasWeapon = false;
        }
    }
    else
    {
        UAnimInstance* Anim = GetMesh()->GetAnimInstance();

        Anim->OnMontageEnded.AddDynamic(this, &ThisClass::CallOnUnequippedEnded);
        Anim->Montage_Play(UnequippedMontage, UnequippedMontagePlayRate);

        Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::NotfiyOnUnequippedWeapon);
    }

}

void AS1EquipmentDefaultBot::EquipWeapon(bool bBeginPlayEquipped)
{
    if (bBeginPlayEquipped)
    {
        Weapon = GetWorld()->SpawnActor<AActor>(WeaponClass);
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r"));
        OnEquippedEnded.Broadcast();
        bHasWeapon = true;
    }
    else
    {
        UAnimInstance* Anim = GetMesh()->GetAnimInstance();

        Anim->OnMontageEnded.AddDynamic(this, &ThisClass::CallOnEquippedEnded);
        Anim->Montage_Play(EquippedMontage, EquippedMontagePlayRate);

        Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::NotfiyOnEquippedWeapon);
    }

}

void AS1EquipmentDefaultBot::RemoveDelegates()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

    Anim->OnMontageEnded.RemoveAll(this);
    Anim->OnPlayMontageNotifyBegin.RemoveAll(this);
}


