
// Copyright (c) S1

#pragma once

#include "Bot/Character/S1EquipmentDefaultBot.h"
#include "Interface/BlockInterface.h"
#include "S1EquipmentBlockBaseBot.generated.h"

UCLASS()
class S1_API AS1EquipmentBlockBaseBot : public AS1EquipmentDefaultBot, public IBlockInterface
{
    GENERATED_BODY()

public:
    AS1EquipmentBlockBaseBot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TryToBlock() override;
	virtual void StartBlcok(EBlockingState BlockingState) override;
	virtual void EndBlcok() override;
	virtual void Blocking(EBlockingState BlockingState) override;

}; 
