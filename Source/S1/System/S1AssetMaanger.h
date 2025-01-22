#pragma once

#include "Engine/AssetManager.h"
#include "S1AssetMaanger.generated.h"

UCLASS(Config = Game)
class US1AssetMaanger : public UAssetManager
{
	GENERATED_BODY()

public:		
	US1AssetMaanger();

	/** 싱글톤 메서드 */
	static US1AssetMaanger& Get();

	/** [THREAD-SAFE] 메모리에 로딩된 에셋 캐싱*/
	void AddLoadedAsset(const UObject* Asset);

	/** 코딩 과정에서 테스트를 위한 함수 */
	static bool TestClone();

	/** FScopeLogTime을 사용할 지 판단하는 함수 */
	static bool ShouldLogAssetLoads();

	/** US1AssetManager가 로딩을 관장한다 */
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/** 정적 로딩으로 BP Class와 Object를 로딩 */
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/*
	 * GameplayTag, Queue, 시점 변경 등을 다양하게 수정
	 * 코어한 부분들을 후킹하는 부분이다
	 */
	virtual void StartInitialLoading() final;

	/*
	 * member variables
	 */
	// GC의 대상
	UPROPERTY()
	TSet<TObjectPtr<const UObject>>LoadedAssets;

	// Object 단위 Locking
	FCriticalSection SyncObject;
};

template<typename AssetType>
AssetType* US1AssetMaanger::GetAsset( const TSoftObjectPtr<AssetType>& AssetPointer , bool bKeepInMemory )
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]."), *AssetPointer.Get());
		}

		// Asset이 메모리에 존재하고 bKeepInMemor가 true라면?
		if (LoadedAsset && bKeepInMemory)
		{
			// 해당 에셋을 불러오는 역할을 수행한다	 
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> US1AssetMaanger::GetSubclass( const TSoftClassPtr<AssetType>& AssetPointer , bool bKeepInMemory )
{
	return TSubclassOf<AssetType>();
}
