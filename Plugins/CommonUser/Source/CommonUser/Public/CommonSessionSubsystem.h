
// Copyright (c) S1

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

/** 전방 선언 */
class APlayerController;

/*
 *  해당 클래스는 맵 간 이동에 관련된 클래스이다
 */
UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
    GENERATED_BODY()

public:
    /** 준비된 MapID (맵 경로) */
    UPROPERTY(BlueprintReadWrite, Category = "Session")
    FPrimaryAssetId MapID;

    /** 전달한 CmdArgs (Experience 이름을 전달, UserFacingExperienceDeifnition) */
    UPROPERTY(BlueprintReadWrite, Category = "Session")
    TMap<FString, FString> ExtraArgs;

    /** ServerTravel에 전달할 최종 URL을 생성한다 */
    FString ConstructTravelURL() const;

    /** MapID -> Text 변환 */
    FString GetMapName() const;

};

/*
 *  S1에서 사용해야 하기 때문에 DLL_EXPORT를 수행해줘야 한다 (COMMONUSER_API)
 */

UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UCommonSessionSubsystem() {}
    
    UFUNCTION(BlueprintCallable, Category = "Session")
    void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

    /*
     * member variables
     */
    /** 맵의 경로 String */
    FString PendingTravelURL;


}; 
