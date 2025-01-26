
// Copyright (c) S1

#include "S1PawnComponent_CharacterParths.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PawnComponent_CharacterParths)

US1PawnComponent_CharacterParths::US1PawnComponent_CharacterParths(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CharacterPartList(this)
{

}

FS1CharacterPartHandle US1PawnComponent_CharacterParths::AddCharacterPart(const FS1CharacterPart& NewPart)
{
    return CharacterPartList.AddEntry(NewPart);
}

FGameplayTagContainer FS1CharacterPartList::CollectCombinedTags() const
{
    FGameplayTagContainer Result;
        
    // Entries를 순회하며,
    for (const FS1AppliedCharacterPartEntry& Entry : Entries)
    {
        // Part Actor가 생성되어 SpawnedComponent에 캐싱되어 있으면
        if (Entry.SpawnedComponent)
        {
            // 해당 Actor의 IGameplayTagAssetInterface를 통해 GameplayTag를 검색
            //  - TaggedActor는 IGameplayTagAssetInterface를 상속받지 않음
            //  - 각 Part에 대해 GameplayTag를 넣고 싶다면 이걸 상속받아 정의
            //      - Lv. 100이상 장착 가능한 장비를 만들고 싶다하면 넣어야함
            if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
            {
                TagInterface->GetOwnedGameplayTags(Result);
            }
        }
    }

    return Result;
}

bool FS1CharacterPartList::SpawnActorForEntry(FS1AppliedCharacterPartEntry& Entry)
{
    bool bCreatedAnyActor = false;
    // 전달된 S1AppliedCharacterPartEntry의 Part Class가 제대로 세팅되어 있다면
    if (Entry.Part.PartClass != nullptr)
    {
        // OwnerComponent의 Owner에 속한 World를 반환
        UWorld* World = OwnerComponent->GetWorld();

        // S1PawnComponent_CharacterParts에 어느 Component에 붙일 것인지 결정
        if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
        {
            // 붙일 Component인 ComponentToAttachTo의 Bone 혹은 ScoketName을 통해 어디에 붙일지 Transform을 계산한다
            const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

            // Actor-Actor의 겷랍이므로, ChildActorComponent를 활용
            UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
            PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
            PartComponent->SetChildActorClass(Entry.Part.PartClass);
            // RegisterComponent를 통해 마지막으로 RenderWorld인 FScene에 변경 내용 전달(혹은 생성)
            PartComponent->RegisterComponent();

            // ChildActorComponent에서 생성한 Actor를 반환하여
            if (AActor* SpawnedActor = PartComponent->GetChildActor())
            {
                // 해당 Actor가 Parent인 S1PawnComponent_CharacterParts의 Owner Actor보다 먼저 Tick이 실행되지 않도록 선행조건을 붙인다
                if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
                {
                    SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
                }
            }
            Entry.SpawnedComponent = PartComponent;
            bCreatedAnyActor = true;
        }
    }

    return bCreatedAnyActor;
}

void FS1CharacterPartList::DestoryActorForEntry(FS1AppliedCharacterPartEntry& Entry)
{
    if (Entry.SpawnedComponent)
    {
        Entry.SpawnedComponent->DestroyComponent();
        Entry.SpawnedComponent = nullptr;
    }
}

FS1CharacterPartHandle FS1CharacterPartList::AddEntry(const FS1CharacterPart& NewPart)
{
    // PawnComponent의 CharacterPartList가 PartHandle를 관리하고, 이를 ControllerComponent_CharacterParts에 전달한다
    FS1CharacterPartHandle Result;
    Result.PartHandle = PartHandleConunter++;

    // Authority가 있다면, AppliedCharacterPartEntry를 Entries에 추가한다
    if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
    {
        FS1AppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
        NewEntry.Part = NewPart;
        NewEntry.PartHandle = Result.PartHandle;

        // 여기서 실제 Actor를 생성하고, OwnerComponent의 Owner Actor에 Actor끼리 RootComponent로 Attach 시킨다
        if (SpawnActorForEntry(NewEntry))
        {
            // BoradcastChanged를 통해, OwnerComponent에서 Owner의 SkeletalMeshComponent를 활용하여, Animation 및 Physics를 Re-Initialize해준다
            OwnerComponent->BroadcastChanged();
        }
    }

    return FS1CharacterPartHandle();
}

void FS1CharacterPartList::RemoveEntry(FS1CharacterPartHandle Handle)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FS1AppliedCharacterPartEntry& Entry = *EntryIt;

        // 제거할 경우, PartHandle을 활용한다
        if (Entry.PartHandle == Handle.PartHandle)
        {
            DestoryActorForEntry(Entry);
        }
    }
}


USceneComponent* US1PawnComponent_CharacterParths::GetSceneComponentToAttachTo() const
{
    // Parent에 SkeletalMeshComponent가 있으면 반환하고
    if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
    {
        return MeshComponent;
    }

    // RootComponent도 확인하고
    else if (AActor* OwnerActor = GetOwner())
    {
        return OwnerActor->GetRootComponent();
    }


    // 그래도 없다면
    return nullptr;
}

USkeletalMeshComponent* US1PawnComponent_CharacterParths::GetParentMeshComponent() const
{
    // Character를 활용하여, 최상위 SkeletalMesh를 반환한다
    if (AActor* OwnerActor = GetOwner())
    {
        if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
        {
            if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
            {
                return MeshComponent;
            }
        }
    }

    return nullptr;
}

void US1PawnComponent_CharacterParths::RemoveCharacterPart(FS1CharacterPartHandle Handle)
{
    CharacterPartList.RemoveEntry(Handle);
}

void US1PawnComponent_CharacterParths::BroadcastChanged()
{
    const bool bReinitPose = true;

    // 현재 Owner의 SkeletalMeshComponent를 반환한다
    if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
    {
        // BodyMeshes를 통해, GameplayTag를 활용하여, 알맞은 
        const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
        USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

        // SkeletalMesh를 초기화 및 Animation 초기화 시켜준다
        MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

        // PhysicsAsset을 초기화해준다
        if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
        {
            MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
        }
    }
}

FGameplayTagContainer US1PawnComponent_CharacterParths::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
    // 현재 장착된 CharacterPartList의 Merge된 Tags를 반환한다
    FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
    if (RequiredPrefix.IsValid())
    {
        // 만약 GameplayTag를 통해 필터링할 경우, 필터링해서 진행
        return Result.Filter(FGameplayTagContainer(RequiredPrefix));
    }
    else
    {
        // 필터링할 GameplayTag가 없으면 그냥 반환
        return Result;
    }
}

