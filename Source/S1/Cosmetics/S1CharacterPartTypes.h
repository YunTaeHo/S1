
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "S1CharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FS1CharacterPartHandle
{
    GENERATED_BODY()

    void Reset()
    {
        PartHandle = INDEX_NONE;
    }

    bool IsValid() const
    {
        return PartHandle != INDEX_NONE;
    }

    UPROPERTY()
    int32 PartHandle = INDEX_NONE;

};

USTRUCT(BlueprintType)
struct FS1CharacterPart
{
    GENERATED_BODY()

    /** Character Part에 대한 정의(메타 데이터 == MetaData) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> PartClass;

    /** Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보(따로 정의되어 있지 않으면 그냥 Root에 붙임) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SocketName;
};