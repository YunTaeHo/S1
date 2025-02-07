
// Copyright (c) S1

#include "S1EquipmentDefaultBot.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentDefaultBot)

AS1EquipmentDefaultBot::AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1EquipmentDefaultBot::SetMovementSpeed(EMoveState MoveState)
{
    float Speed = 0.f;

    switch (MoveState)
    {

    case EMoveState::Walking:
        Speed = 100.f;
        break;
    case EMoveState::Jogging:
        Speed = 300.f;
        break;
    case EMoveState::Sprinting:
        Speed = 500.f;
        break;

    case EMoveState::Idle:
    default:
        break;
    }

    GetCharacterMovement()->MaxWalkSpeed = Speed;
}


void AS1EquipmentDefaultBot::CallOnAttackEnd()
{
    OnAttackEnded.Broadcast();
    RemoveDelegates();
}

void AS1EquipmentDefaultBot::CallOnEquippedEnded(UAnimMontage* Montage, bool bInterrupted)
{
    OnEquippedEnded.Broadcast();
    RemoveDelegates();
}

void AS1EquipmentDefaultBot::CallOnUnequippedEnded(UAnimMontage* Montage, bool bInterrupted)
{
    OnUnEquippedEnded.Broadcast();
    RemoveDelegates();
}

void AS1EquipmentDefaultBot::NotfiyOnEquippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == TEXT("Equipped"))
    {
        Weapon = GetWorld()->SpawnActor<AActor>(WeaponClass);
        Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r"));
    }
    
}

void AS1EquipmentDefaultBot::NotfiyOnUnequippedWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName == TEXT("Unequipped"))
    {
        if (Weapon)
        {
            Weapon->Destroy();
        }
    }
}

void AS1EquipmentDefaultBot::UnequipWeapon()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

    Anim->OnMontageEnded.AddDynamic(this, &ThisClass::CallOnUnequippedEnded);
    Anim->Montage_Play(UnequippedMontage, UnequippedMontagePlayRate);

    Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::NotfiyOnUnequippedWeapon);
}

void AS1EquipmentDefaultBot::EquipWeapon()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

    Anim->OnMontageEnded.AddDynamic(this, &ThisClass::CallOnEquippedEnded);
    Anim->Montage_Play(EquippedMontage, EquippedMontagePlayRate);

    Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::NotfiyOnEquippedWeapon);
}

void AS1EquipmentDefaultBot::RemoveDelegates()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();

    Anim->OnMontageEnded.RemoveAll(this);
    Anim->OnPlayMontageNotifyBegin.RemoveAll(this);
}


