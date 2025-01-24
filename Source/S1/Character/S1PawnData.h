
// Copyright (c) S1

#pragma once

#include "Engine/DataAsset.h"
#include "Camera/S1CameraMode.h"
#include "Input/S1InputConfig.h"
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

    /** Camera Mode */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|Camera")
    TSubclassOf<US1CameraMode> DefaultCameraMode;

    /** Input Config는 컨트롤러가 Possess한 플레이어가 사용하게 된다 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "S1|InputConfig")
    TObjectPtr<US1InputConfig> InputConfig;

}; 
