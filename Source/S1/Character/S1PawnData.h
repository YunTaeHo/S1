
// Copyright (c) S1

#pragma once

#include "Engine/DataAsset.h"
#include "S1PawnData.generated.h"

UCLASS()
class S1_API US1PawnData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    US1PawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** @TODO - Pawn에 대한 정보들을 나중에 전부 세팅해주도록 하자 */

    /** Pawn의 Class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Pawn")
    TSubclassOf<APawn> PawnClass;

}; 
