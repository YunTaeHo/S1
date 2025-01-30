
// Copyright (c) S1

#pragma once

#include "S1CharacterPartTypes.h"
#include "S1CosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "S1PawnComponent_CharacterParths.generated.h"

/** 인스턴스화된 Character Part의 단위 */
USTRUCT(BlueprintType)
struct FS1AppliedCharacterPartEntry
{
    GENERATED_BODY()

    /** Character Part의 정의(메타 데이터) */
    UPROPERTY()
    FS1CharacterPart Part;

    /** S1CharacterPartList에서 할당 받은 Part 핸들값 (FS1ControllerCharacterPartEntry의 Handle 값과 같아야함 -> 같으면 같은 Part) */
    UPROPERTY()
    int32 PartHandle = INDEX_NONE;

    /** 인스턴스화 된 Character Part용 Actor */
    UPROPERTY()
    TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** S1PawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FS1CharacterPartList
{
    GENERATED_BODY()

    FS1CharacterPartList()
        : OwnerComponent(nullptr)
    {}

    FS1CharacterPartList(US1PawnComponent_CharacterParths* InOwnerComponent)
        : OwnerComponent(InOwnerComponent)
    {}

    FGameplayTagContainer CollectCombinedTags() const;


    bool SpawnActorForEntry(FS1AppliedCharacterPartEntry& Entry);
    void DestoryActorForEntry(FS1AppliedCharacterPartEntry& Entry);

    FS1CharacterPartHandle AddEntry(const FS1CharacterPart& NewPart);
    void RemoveEntry(FS1CharacterPartHandle Handle);

    /** 현재 인스턴스화된 Character Part */
    UPROPERTY()
    TArray<FS1AppliedCharacterPartEntry> Entries;

    /** FS1CharacterPartList의 Owner */
    UPROPERTY()
    TObjectPtr<US1PawnComponent_CharacterParths> OwnerComponent;

    /** PartHandle의 값을 할당 및 관리하는 변수(고유값, ControllerComponent에서 사용) */
    int32 PartHandleConunter = 0;
};

/*
 * PawnComponent로, Character Parts를 인스턴스화하여 관리한다
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class S1_API US1PawnComponent_CharacterParths : public UPawnComponent
{
    GENERATED_BODY()

public:
    US1PawnComponent_CharacterParths(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Cosmetics")
    FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

    FS1CharacterPartHandle AddCharacterPart(const FS1CharacterPart& NewPart);
    void BroadcastChanged();
    USceneComponent* GetSceneComponentToAttachTo() const;
    USkeletalMeshComponent* GetParentMeshComponent() const;

    void RemoveCharacterPart(FS1CharacterPartHandle Handle);

    /** 인스턴스화 된 Character Parts */
    UPROPERTY()
    FS1CharacterPartList CharacterPartList;

    /** 애니메이션 적용을 위한 메시와 연결고리 */
    UPROPERTY(EditAnywhere, Category = "Cosmetics")
    FS1AnimBodyStyleSelectionSet BodyMeshes;

}; 
