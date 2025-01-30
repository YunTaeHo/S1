
// Copyright (c) S1

#pragma once

#include "CommonGameInstance.h"
#include "S1GameInstance.generated.h"


UCLASS()
class S1_API US1GameInstance : public UCommonGameInstance
{
    GENERATED_BODY()

public:
    US1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UGameInstance`s interfaces
     */
    virtual void Init() override;
    virtual void Shutdown() override;

}; 
