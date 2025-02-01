
// Copyright (c) S1

#include "S1GameplayTagStack.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1GameplayTagStack)


void FS1GameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        return;
    }

    if (StackCount > 0)
    {
        // Stacks 을 순회한다
        // O(n) 이지만 Stack이 1000개는 안들어갈 것 같기 때문에 모두 순회해도 될 것 같다
        for (FS1GameplayTagStack& Stack : Stacks)
        {
            if (Stack.Tag == Tag)
            {
                const int32 NewCount = Stack.StackCount + StackCount;
                Stack.StackCount = NewCount;
                TagToCountMap[Tag] = NewCount;
                return;
            }
        }

        // 만약 for문으로 다 순회했는데 해당 태그를 못했다면 추가해주자
        FS1GameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);


        TagToCountMap.Add(Tag, StackCount);
    }
}

void FS1GameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
    if (!Tag.IsValid())
    {
        return;
    }

    if (StackCount > 0)
    {
        // Iterator를 순회하면서 지워준다(지워도 순회는 유지되기 때문에 반복자 사용)
        for (auto It = Stacks.CreateIterator(); It; ++It)
        {
            FS1GameplayTagStack& Stack = *It;
            if(Stack.Tag == Tag)
            {
                // 인자로 들어온 StackCount가 더 많다면
                if (Stack.StackCount <= StackCount)
                {
                    // 해당 반복자를 이용해 배열에서 빼주고, 룩업 테이블에서도 제거시켜준다
                    It.RemoveCurrent();
                    TagToCountMap.Remove(Tag);
                }
                // 인자로 들어온 숫자만큼 제거 시켜준다
                else
                {
                    const int32 NewCount = Stack.StackCount - StackCount;
                    Stack.StackCount = NewCount;
                    TagToCountMap[Tag] = NewCount;
                }
                return;
            }
        }
    }
}
