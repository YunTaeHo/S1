
// Copyright (c) S1

#include "S1ExperienceActionSet.h"
#include "GameFeatureAction.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ExperienceActionSet)

US1ExperienceActionSet::US1ExperienceActionSet()
    : Super()
{
}

#if WITH_EDITORONLY_DATA
void US1ExperienceActionSet::UpdateAssetBundleData()
{
    Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif
