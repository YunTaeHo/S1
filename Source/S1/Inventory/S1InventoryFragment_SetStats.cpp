
// Copyright (c) S1

#include "S1InventoryFragment_SetStats.h"
#include "S1InventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1InventoryFragment_SetStats)

void US1InventoryFragment_SetStats::OnInstanceCreated(US1InventoryItemInstance* Instance) const
{
	// Fragment에서 선언한 Key, Value 값을 이용해 Instance에 StackTag를 넣어준다 
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}
