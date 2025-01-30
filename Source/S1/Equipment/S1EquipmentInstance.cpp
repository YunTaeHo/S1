
// Copyright (c) S1

#include "S1EquipmentInstance.h"
#include "GameFramework/Character.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1EquipmentInstance)

US1EquipmentInstance::US1EquipmentInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

APawn* US1EquipmentInstance::GetTypePawn(TSubclassOf<APawn> PawnType) const
{
    APawn* Result = nullptr;
    if (UClass* ActualPawnType = PawnType)
    {
        if (GetOuter()->IsA(ActualPawnType))
        {
            Result = Cast<APawn>(GetOuter());
        }
    }
    return Result;
}

void US1EquipmentInstance::SpawnEquipmentActors(const TArray<FS1EquipmentActorToSpawn>& ActorsToSpawn)
{
    // Outer의 Pawn을 가져와서
    if (APawn* OwningPawn = GetPawn())
    {
        // 현재 Owner인 Pawn의 RootComponent를 AttachTarget 대상으로 한다
        USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
        if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
        {
            AttachTarget = Character->GetMesh();
        }

        for (const FS1EquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
        {
            // SpawnInfo기반으로 생성
            AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
            NewActor->FinishSpawning(FTransform::Identity, /** bIsDefaultTransform=*/ true);

            // Actor의 RelativeTransform을 AttachTransform으로 설정
            NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

            // AttachTarget에 붙이자 (Actor -> Actor)
            NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
        
            SpawnedActors.Add(NewActor);
        }
    }
}


void US1EquipmentInstance::DestroyEquipmentActors()
{
    // 갑옷, 장갑, 상의, 하의 등을 모두 지워준다
    for (AActor* Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }
}

void US1EquipmentInstance::OnEquipped()
{
    K2_OnEquipped();
}

void US1EquipmentInstance::OnUnequipped()
{
    K2_OnUnequipped();
}

APawn* US1EquipmentInstance::GetPawn() const
{
    // EquipmentInstance 생성 시, Outer를 반드시 Pawn이어야 함(FS1EquipmentList)
    return Cast<APawn>(GetOuter());
}


