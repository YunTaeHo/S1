#include "S1AssetMaanger.h"
#include "S1LogChannel.h"
#include "S1GameplayTags.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1AssetMaanger)

US1AssetMaanger::US1AssetMaanger()
	: Super()
{

}

US1AssetMaanger& US1AssetMaanger::Get()
{
	check(GEngine);

	if (US1AssetMaanger* Singleton = Cast<US1AssetMaanger>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogS1, Fatal, TEXT("Invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be S1AssetManager"));

	return *NewObject<US1AssetMaanger>();
}

void US1AssetMaanger::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		// 쓰레드 안정성을 위해 임계 영역의 Lock을 이용한다
		FScopeLock Lock(&SyncObject);
		// 로딩된 에셋을 캐싱해둔다
		LoadedAssets.Add(Asset);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
bool US1AssetMaanger::TestClone()
{
	static bool bTest = ShouldLogAssetLoads();
	return false;
}

bool US1AssetMaanger::ShouldLogAssetLoads()
{
	// 테스트 커맨드 라인
	// 맵 간 이동을 할 때 
	// 속성 - 명령 인수에 맵 트랜지션 때 추가
	// 프로세스 <-> 프로세스 간의 커맨드 라인 설정
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param( CommandLineContent , TEXT( "LogAssetLoads" ) );
	return bLogAssetLoads;
}

PRAGMA_ENABLE_OPTIMIZATION

UObject* US1AssetMaanger::SynchronousLoadAsset( const FSoftObjectPath& AssetPath )
{
	// 버벅거림을 방지하기 위해 Valid 체크(불필요한 load asset 호출 방지)
	if (AssetPath.IsValid())
	{
		// FScopeLogTime 확인
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronouse loaded assets [%s]"), *AssetPath.ToString(), nullptr, FScopeLogTime::ScopeLog_Seconds));
		}

		// AssetManager가 존재한다면?
		if (UAssetManager::IsValid())
		{
			// StreamableManager로 정적로딩 수행
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// StaticLoadObject 수행(한 번 로딩되어있다면, 캐싱된 FindObject 반환
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void US1AssetMaanger::StartInitialLoading()
{
	Super::StartInitialLoading();

	// 로깅한다
	UE_LOG(LogS1, Display, TEXT("US1AssetMaanger::StartInitialLoading"));

	// @TODO - 나중에 STARTUP_JOB으로 구현
	FS1GameplayTags::InitializeNativeTags();
}
