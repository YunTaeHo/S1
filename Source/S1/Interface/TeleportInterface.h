// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "TeleportInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UTeleportInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class S1_API ITeleportInterface
{
	GENERATED_BODY()

public:
	/** 해당 함수를 통해 텔레포트를 할 수 있습니다 */
	virtual void Teleport(FVector Location) = 0;
};
