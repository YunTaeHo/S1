
// Copyright (c) S1

#include "CommonSessionSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonSessionSubsystem)


void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}

	//UGameplayStatics::OpenLevel(GetWorld(), *Request->GetMapName());
	// 받은 Request에서 MapID와 ExtraArgs를 통해 URL을 생성하고, MapLoad를 수행한다
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}


FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	// 최종 전달되는 Args
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		/*
		 *	?를 separate로 복수개의 ExtraArg를 추가함 :
		 *	- Key 값 유무에 따라, =(assignment)를 통해 알맞는 CmdArg를 생성
		 */
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager -> MapId -> FAssetData
	// - 이를 통해 PackageName  -> Map 경로를 반환
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}

	return FString();
}
