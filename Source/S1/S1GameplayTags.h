
// Copyright (c) S1

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"
/** 전방 선언 */
class UGmaeplayTagsManager;

/*
 *  S1GameplayTags
 *  - Singleton containing native gameplay tags
 */
struct FS1GameplayTags
{
    /*
     *  static methods
     */
    static const FS1GameplayTags& Get() { return GameplayTags;  }
    static void InitializeNativeTags();

    /*
     *  member methods
     */
    void AddAllTags(UGameplayTagsManager& Manager);
    void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

    /*
     *  초기과 과정의 단계를 의미하는 태그
     *  - GameInstance의 초기화 과정에 UGameFrameworkComponentManmager의 RegisterInitState로 등록되어 선형적으로(linear)하게 업데이트 된다
     *  - 이 초기화 GameplayTag는 게임의 Actor 사이에 공유되며, GameFrameworkInitStateInterface 상속받은 클래스는 
            초기화 상태(Init State)를 상태머신(State Machine)과 같이 관리 가능하다
     */
    FGameplayTag InitState_Spawned;
    FGameplayTag InitState_DataAvailable;
    FGameplayTag InitState_DataInitialized;
    FGameplayTag InitState_GameplayReady;

    FGameplayTag InputTag_Move;
    FGameplayTag InputTag_Look_Mouse;


private:
    static FS1GameplayTags GameplayTags;
};
