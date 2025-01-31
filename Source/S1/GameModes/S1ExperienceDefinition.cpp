#include "S1ExperienceDefinition.h"
#include "S1ExperienceActionSet.h"
#include "GameFeatureAction.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1ExperienceDefinition)

US1ExperienceDefinition::US1ExperienceDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITORONLY_DATA
void US1ExperienceDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			// 임의적으로 호출해 AssetBundleData에 추가해준다
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif