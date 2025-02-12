// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class S1_API ITeamInterface
{
	GENERATED_BODY()

public:
	/** 팀 이름이 같은 지 확인할 수 있습니다 */
	virtual int32 GetTeamNumber() = 0;
};
