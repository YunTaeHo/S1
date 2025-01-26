
// Copyright (c) S1

#include "S1ControllerComponent_CharacterParts.h"
#include "S1PawnComponent_CharacterParths.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ControllerComponent_CharacterParts)

US1ControllerComponent_CharacterParts::US1ControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void US1ControllerComponent_CharacterParts::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (AController* OwningController = GetController<AController>())
        {
            OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
        }
    }
}

void US1ControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    RemoveAllCharacterParts();
    Super::EndPlay(EndPlayReason);
}

void US1ControllerComponent_CharacterParts::AddCharacterPart(const FS1CharacterPart& NewPart)
{
    AddCharacterPartInternal(NewPart);
}

void US1ControllerComponent_CharacterParts::AddCharacterPartInternal(const FS1CharacterPart& NewPart)
{
    FS1ControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
    NewEntry.Part = NewPart;

    if (US1PawnComponent_CharacterParths* PawnCustomizer = GetPawnCustomizer())
    {
        NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
    }
}

US1PawnComponent_CharacterParths* US1ControllerComponent_CharacterParts::GetPawnCustomizer()
{
    // Controller의 PAwn을 가져와1
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // 블루프린트에서 해당 Pawn이 US1PawnComponent_CharacterParths를 추가해야 찾을 수 있다
        return ControlledPawn->FindComponentByClass<US1PawnComponent_CharacterParths>();
    }

    return nullptr;
}

void US1ControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
    if (US1PawnComponent_CharacterParths* PawnCustomizer = GetPawnCustomizer())
    {
        for (FS1ControllerCharacterPartEntry& Entry : CharacterParts)
        {
            PawnCustomizer->RemoveCharacterPart(Entry.Handle);
        }
    }
    CharacterParts.Reset();
}

void US1ControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    // 이전 Pawn 비워주고
    if (US1PawnComponent_CharacterParths* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<US1PawnComponent_CharacterParths>() : nullptr)
    {
        for (FS1ControllerCharacterPartEntry& Entry : CharacterParts)
        {
            OldCustomizer->RemoveCharacterPart(Entry.Handle);
            Entry.Handle.Reset();
        }
    }

    // 새로운 Pawn 추가한다
    if (US1PawnComponent_CharacterParths* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<US1PawnComponent_CharacterParths>() : nullptr)
    {
        for (FS1ControllerCharacterPartEntry& Entry : CharacterParts)
        {
            check(!Entry.Handle.IsValid());
            Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
        }
    }
}


