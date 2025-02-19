
// Copyright (c) S1

#include "S1GameplayTags.h"
#include "S1LogChannel.h"
#include "GameplayTagsManager.h"

FS1GameplayTags FS1GameplayTags::GameplayTags;

void FS1GameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FS1GameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	/*
	 *	GameFrameworkComponentManager init state tags
	 */
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/repliacted and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: Actor/Component is fully ready for active gameplay");

	/*
	 *	Enhanced Input Tags
	 */
	AddTag(InputTag_Move, "InputTag.Move", "");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "");
	AddTag(InputTag_Jump, "InputTag.Jump", "");
	AddTag(InputTag_Zoom, "InputTag.Zoom", "");

	AddTag(InputTag_Sprint, "InputTag.Sprint", "");
	AddTag(InputTag_Crouch, "InputTag.Crouch", "");


	AddTag(Status_Death_Dying, "Status.Death.Dying", "");
	AddTag(Status_Death_Dead, "Status.Death.Dead", "");

}


void FS1GameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString("(Native) ") + FString(TagComment));
}

