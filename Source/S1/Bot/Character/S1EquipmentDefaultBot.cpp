
// Copyright (c) S1

#include "S1EquipmentDefaultBot.h"
#include "Components/SkeletalMeshComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentDefaultBot)

AS1EquipmentDefaultBot::AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void AS1EquipmentDefaultBot::SetMovementSpeed()
{
}

void AS1EquipmentDefaultBot::CallOnAttackEnd()
{
    OnAttackEnded.Broadcast();
}

void AS1EquipmentDefaultBot::CallOnEquippedEnded()
{
    OnEquippedEnded.Broadcast();
}

void AS1EquipmentDefaultBot::CallOnUnequippedEnded()
{
    OnUnEquippedEnded.Broadcast();
}

void AS1EquipmentDefaultBot::UnequipWeapon()
{

}

void AS1EquipmentDefaultBot::EquipWeapon()
{
    Weapon = GetWorld()->SpawnActor<AActor>(WeaponClass);
    Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("weapon_r"));
}


