
// Copyright (c) S1

#pragma once

#include "GameplayTagContainer.h"
#include "S1GameplayTagStack.generated.h"

/*
 * Tag와 Count를 이용해 하나의 스택을 만들어준다
 * - ex) Ammo_Pistol은 12발의 탄창을 사용할 수 있다(Tag + Count)
 */
USTRUCT(BlueprintType)
struct FS1GameplayTagStack
{
	GENERATED_BODY()

    FS1GameplayTagStack() {}
    FS1GameplayTagStack(FGameplayTag InTag, int32 InStackCount)
        : Tag(InTag)
        , StackCount(InStackCount)
    {}

    UPROPERTY()
    FGameplayTag Tag;

    UPROPERTY()
    int32 StackCount;
};

/** S1GameplayTagStack 컨테이너 */
USTRUCT(BlueprintType)
struct FS1GameplayTagStackContainer
{
    GENERATED_BODY()

    FS1GameplayTagStackContainer() {}

    /** GameplayTag를 이용해 StackCount를 생성/제거 한다*/
    void AddStack(FGameplayTag Tag, int32 StackCount);
    void RemoveStack(FGameplayTag Tag, int32 StackCount);

    /** GameplayTag를 이용해 count를 가져온다 */
    int32 GetStackCount(FGameplayTag Tag) const
    {
        return TagToCountMap.FindRef(Tag);
    }

    /** GameplayTagStackContainer 안에 해당 태그를 사용하는 지 체크 */
    bool ContainsTag(FGameplayTag Tag) const
    {
        return TagToCountMap.Contains(Tag);
    }

    /** Stack을 모아놓은 배열 */
    TArray<FS1GameplayTagStack> Stacks;

    /** 룩업 테이블을 이용해 O(1)의 시간복잡도로 Count를 가져올 수 있도록 사용 */
    TMap<FGameplayTag, int32> TagToCountMap;
};
