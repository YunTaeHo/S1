
// Copyright (c) S1

#include "UIExtensionSystem.h"
#include "Blueprint/UserWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(UIExtensionSystem)

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

void FUIExtensionPointHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtensionPoint(*this);
	}
}

bool FUIExtensionPoint::DoesExtensionPassContract(const FUIExtension* Extension) const
{
	// WidgetClass
	if (UObject* DataPtr = Extension->Data)
	{
		const bool bMatchContext =
			// ContextObject와 Extension->ContextObject가 둘다 nullptr로 세팅되었는지?
			(ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull()) ||
			// 아니면 실제 ContextObject와 Extension->ContextObject가 같은지? (LocalPlayer? PlayerState?)
			(ContextObject == Extension->ContextObject);

		if (bMatchContext)
		{
			// DataClass (WidgetClass 추출)
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			// AllowedDataClasses순회하며
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				// Child/Interface인지 확인
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}

	return false;
}



void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		// 반드시 해당 ExtensionHandle이 UUIExtensionSubsystem과 같은지 확인
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("Trying to unregister an extension that`s not from this extension subsystem."));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;

		// Extension의 PointTag를 통해 ExtensionMap에서 해당 Slot에 있는지 찾은 후 제거
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			// Extension 제거, NotfiyExtension
			NotifyExtensionPointsOfExtension(EUIExtensionAction::Removed, Extension);

			ListPtr->RemoveSwap(Extension);
			if (ListPtr->Num() == 0)
			{
				// Num == 0이면 Map에서도 제거 진행
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
	
}

void UUIExtensionSubsystem::UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle)
{
	// 이전 UnregisterExtension()와 거의 흡사
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);
		const TSharedPtr<FUIExtensionPoint> ExtensionPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(ExtensionPoint->ExtensionPointTag))
		{
			ListPtr->RemoveSwap(ExtensionPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionMap.Remove(ExtensionPoint->ExtensionPointTag);
			}
		}
	}
}



FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	if (!Data)
	{
		return FUIExtensionHandle();
	}

	// ExtensionPointTag를 기반하여, ExtensionList를 추출
	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	// 새로운 UIExtension을 추가 진행
	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	// Extension이 추가되었으니 Added로 NotifyExtensionPointsOfExtension 실행
	NotifyExtensionPointsOfExtension(EUIExtensionAction::Added, Entry);

	return FUIExtensionHandle(this, Entry);
}


FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(ExtensionPointTag, nullptr, ExtensionPointTagMatchType, AllowedDataClasses, ExtensionCallback);
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionPointHandle();
	}

	// 위젯을 추가할 콜백 함수가 없다면
	if (!ExtensionCallback.IsBound())
	{
		return FUIExtensionPointHandle();
	}

	// 허용된 위젯이 없다면
	if (AllowedDataClasses.Num() == 0)
	{
		return FUIExtensionPointHandle();
	}

	// ExtensionPoinMap에 새로운 Entry를 등록
	// - List 내에 중복 체크는 따로 하지 않는다(Register/Unregister 쌍을 잘 맞춰야함)
	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(ExtensionPointTag);
	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef(MakeShared<FUIExtensionPoint>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->ExtensionPointTagMatchType = ExtensionPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);

	// ExtensionPoint가 추가되었으니, NotifyExtensionPointOfExtensions 호출
	NotifyExtensionPointOfExtensions(Entry);

	return FUIExtensionPointHandle(this, Entry);
}


FUIExtensionRequest UUIExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension)
{
	FUIExtensionRequest Request;
	Request.ExtensionHandle = FUIExtensionHandle(this, Extension);
	Request.ExtensionPointTag = Extension->ExtensionPointTag;
	Request.Priority = Extension->Priority;
	Request.Data = Extension->Data;
	Request.ContextObject = Extension->ContextObject.Get();

	return Request;
}

// ExtensionPoint에 변화가 일어났을 때 호출
void UUIExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint)
{

	// ExtensionPoint의 ExtensionPoinTag(Slot)을 순회하며
	// - A.B.C ->A.B -> A 기준으로 부모를 타고 넘어가 순회
	for (FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		// ExtensionMap을 통해 ExtensionPointTag에 매칭되는 ExtensionList를 가져오자
		if (const FExtensionList* ListPtr = ExtensionMap.Find(Tag))
		{
			// ExtensionList를 순회하며
			FExtensionList ExtensionArray(*ListPtr);
			for (const TSharedPtr<FUIExtension>& Extension : ExtensionArray)
			{
				// 현재 ExtensionPoint가 순회하는 Extension에 허용되는지 확인
				if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
				{
					// UIExtensionRequest를 만들고, ExtensionPoint의 UIExtensionPointWidget의 Add를 실행
					FUIExtensionRequest Request = CreateExtensionRequest(Extension);
					ExtensionPoint->Callback.ExecuteIfBound(EUIExtensionAction::Added, Request);
				}
			}
		}

		// ExactMath라면 굳이 GameplayTag의 Parent를 순회할 필요가 없다
		if (ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::ExactMatch)
		{
			break;
		}
	}


}

// Extensio에 변화가 일어났을 때 호출
void UUIExtensionSubsystem::NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension)
{
	// bOnInitialTag를 활용해 ExactMath 로직을 만들자
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Extension->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			// ExtensionPoint를 순회
			FExtensionPointList ExtensionPointArray(*ListPtr);
			for (const TSharedPtr<FUIExtensionPoint>& ExtensionPoint : ExtensionPointArray)
			{
				/** ExactMatch용 + PartialMath용 */
				if (bOnInitialTag ||
					(ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::PartialMatch))
				{
					if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
					{
						FUIExtensionRequest Request = CreateExtensionRequest(Extension);
						ExtensionPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}

		// 첫 순회가 Exact가 이므로 bOnInitialTag를 false로 둔다
		// 이후 Partial로 계속 순회해서 맞춰준다
		bOnInitialTag = false;
	}
}

