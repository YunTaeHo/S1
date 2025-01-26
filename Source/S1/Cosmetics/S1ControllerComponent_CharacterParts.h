
// Copyright (c) S1

#pragma once

#include "S1CharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "S1ControllerComponent_CharacterParts.generated.h"

/** 전방 선언 */
class US1PawnComponent_CharacterParths;


/** ControllerComponent가 소유하는 Character Parts */
USTRUCT()
struct FS1ControllerCharacterPartEntry
{
    GENERATED_BODY()

    /** Character Part에 대한 정의(메타 데이터 == MetaData) */
    UPROPERTY(EditAnywhere)
    FS1CharacterPart Part;

    /** Character Part 핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) Character Part 핸들값 */
    FS1CharacterPartHandle Handle;
};

/*
 * Controller가 Pawn을 Possess했을 때, Pawn에 어떤 Cosmetic Actor를 생성할 지 결정 :
 *  - 캐릭터 파츠를 유저 관점에서 관리하는 컴포넌트
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class S1_API US1ControllerComponent_CharacterParts : public UControllerComponent
{
    GENERATED_BODY()

public:
    US1ControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintCallable, Category = "Cosmetics")
    void AddCharacterPart(const FS1CharacterPart& NewPart);

    void AddCharacterPartInternal(const FS1CharacterPart& NewPart);
    US1PawnComponent_CharacterParths* GetPawnCustomizer();

    void RemoveAllCharacterParts();

    /** Possess되었을 때 AddPart가 구현될 수 있도록 설정 */
    UFUNCTION()
    void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);


    UPROPERTY(EditAnywhere, Category = "Cosmetics")
    TArray<FS1ControllerCharacterPartEntry> CharacterParts;


}; 
