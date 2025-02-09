
// Copyright (c) S1

#include "S1EquipmentDefaultBot.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ClientUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentDefaultBot)

AS1EquipmentDefaultBot::AS1EquipmentDefaultBot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Widget(3D) 생성
    Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Widget"));
    Widget->SetupAttachment(GetMesh());
}

void AS1EquipmentDefaultBot::BeginPlay()
{
    Super::BeginPlay();

    // Enemy의 Widget을 추가해주도록 하자
    if (HeathBarWidget)
    {
        UClientUserWidget* WidgetTemp = CreateWidget<UClientUserWidget>(GetWorld(), HeathBarWidget);
        WidgetTemp->SetOwner(this);
     
        Widget->SetWidget(WidgetTemp);
    }
}

void AS1EquipmentDefaultBot::SetMovementSpeed(EMovementState MoveState)
{
    switch (MoveState)
    {
    case EMovementState::Idle:
        GetCharacterMovement()->MaxWalkSpeed = MovementState.IdleSpeed;
        break;
    case EMovementState::Walking:
        GetCharacterMovement()->MaxWalkSpeed = MovementState.WalkSpeed;
        break;
    case EMovementState::Jogging:
        GetCharacterMovement()->MaxWalkSpeed = MovementState.JogSpeed;
        break;
    case EMovementState::Sprinting:
        GetCharacterMovement()->MaxWalkSpeed = MovementState.SprintSpeed;
        break;
    default:
        break;
    }
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


