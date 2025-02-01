
// Copyright (c) S1

#include "S1PawnHandler.h"
#include "S1LogChannel.h"
#include "S1GameplayTags.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PawnHandler)

/** feature name은 component 단위이기 때문에 component를 빼고 PawnHandler만 넣었다 */
const FName US1PawnHandler::NAME_ActorFeatureName("PawnHandler");

US1PawnHandler::US1PawnHandler(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 초기화와 관련되어있기 때문에 Tick을 무조건 없애주도록 한다
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;
}

PRAGMA_DISABLE_OPTIMIZATION
void US1PawnHandler::SetPawnData(const US1PawnData* InPawnData)
{
    // Pawn에 Authority가 없을 경우, SetPawnData는 진행하지 않음
    // PawnData 존재 시 진행하지 않음
    APawn* Pawn = GetPawnChecked<APawn>();
    if (Pawn->GetLocalRole() != ROLE_Authority || PawnData)
    {
        return;
    }

    // PawnData 업데이트
    PawnData = InPawnData;
}

void US1PawnHandler::ForceUpdateInitState()
{
    // ForceUpdate로 다시 InitState 상태 변환 시작 (연결 고리)
    CheckDefaultInitialization();
}

void US1PawnHandler::InitializeAbilitySystem(US1AbilitySystemComponent* InASC, AActor* InOwnerActor)
{
    check(InASC && InOwnerActor);

    if (AbilitySystemComponent == InASC)
    {
        return;
    }

    if (AbilitySystemComponent)
    {
        UninitializeAbilitySystem();
    }

    APawn* Pawn = GetPawnChecked<APawn>();
    AActor* ExistingAvatar = InASC->GetAvatarActor();
    check(!ExistingAvatar);

    // ASC를 업데이트하고, InitAbilityActorInfo를 Pawn과 같이 호출하여, AvatarActor를 Pawn으로 업데이트 해준다
    AbilitySystemComponent = InASC;
    AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

    // OnAbilitySystemInitialized에 바인딩된 Delegate 호출
    OnAbilitySystemInitialized.Broadcast();
}

void US1PawnHandler::UninitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
    {
        // OnAbilitySystemUninitialized에 바인딩된 Delegate 호출
        OnAbilitySystemUninitialized.Broadcast();
    }

    AbilitySystemComponent = nullptr;
}

void US1PawnHandler::OnRegister()
{
    Super::OnRegister();

    // 올바론 Actor에 등록되었는 지 확인
    {
        if (!GetPawn<APawn>())
        {
            UE_LOG(LogS1, Error, TEXT("This component has been added to a BP whose base class is not a Pawn!"));
            return;
        }
    }

    // GameFrameworkComponentManager에 InitState 사용을 위해 등록 진행:
    // - 등록은 상속받았던 IGameFrameworkInitStateInterface 메서드 RegisterInitStateFeature()를 활용
    RegisterInitStateFeature();

    // 디버깅을 위한 함수
    UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}
PRAGMA_ENABLE_OPTIMIZATION


void US1PawnHandler::BeginPlay()
{
    Super::BeginPlay();

    // Actor에 등록된 모든 Feature Component의 InitState 상태를 관찰(NAME_None, FGameplayTag()), false -> 바로 호출하지 않는다
    BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

    //  1. CanChangeInitState로 상태 변환 가능성 유무 판단
    //  2. HandleChangeInitState로 내부 상태 변경(Feature Component)
    //  3. BindOnActorInitStateChanged로 Bind된 Delegate를 조건에 맞게 호출해줌
    //      - S1PawnExtensionComponent의 경우, 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged()가 호출됨
    ensure(TryToChangeInitState(FS1GameplayTags::Get().InitState_Spawned));

    // 해당 함수를 오버라이딩해서 ForceUpdate를 진행시킨다
    CheckDefaultInitialization();
}

void US1PawnHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // OnRegister에서 했던 Register를 지워주는 역할
    UnregisterInitStateFeature();

    Super::EndPlay(EndPlayReason);
}

void US1PawnHandler::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
    // 내가 아닌 누군가 InitState할 시 작동
    if (Params.FeatureName != NAME_ActorFeatureName)
    {
        
        // 지속적으로 Sync를 맞추기위해 FeatureState == InitState_DataAvailable를 구분해준다
        const FS1GameplayTags& InitTags = FS1GameplayTags::Get();
        if (Params.FeatureState == InitTags.InitState_DataAvailable)
        {
            // CheckDefaultInitialization를 호출해, 모두가 InitState_DataAvailable일 때 InitState 할 수 있도록 실행
            CheckDefaultInitialization();
        }
    }
}

PRAGMA_DISABLE_OPTIMIZATION
bool US1PawnHandler::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
    check(Manager);

    APawn* Pawn = GetPawn<APawn>();
    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();

    // InitState_Spawned 초기화
    if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
    {
        // Pawn이 설정되어있으면 바로 Spawned로 넘어감
        if (Pawn)
        {
            return true;
        }
    }

    // Spawned -> DataAvailable
    if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
    {
        // PawnData가 설정되어있지 않다면 InitState를 작동시키지 않는다
        if (!PawnData)
        {
            return false;
        }

        // LocallyControlled인 Pawn이 Controller가 없으면 에러
        const bool bHasAuthority = Pawn->HasAuthority();
        const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
        if (bHasAuthority || bIsLocallyControlled)
        {
            // 아직 Possess가 안되어있을 때 
            if (!GetController<AController>())
            {
                return false;
            }
        }

        return true;
    }

    // DataAvailable -> DataInitialized
    if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
    {
        // Actor에 바인딩된 모든 Feature들이 DataAvailable 상태일 때, DataInitialized로 넘어감
        // - HaveAllFeatureReachedInitState 확인
        return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
    }

    // DataInitialized -> GameplayReady
    if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
    {
        return true;
    }

    return false;
}
PRAGMA_ENABLE_OPTIMIZATION

/** ForceUpdate */
void US1PawnHandler::CheckDefaultInitialization()
{
    // 초기화를 전부 관장하는 컴포넌트이다
    // 그렇기 때문에 나를 제외한 Feature Component들을 모두 CheckDefaultInitialization를 호출해준다
    CheckDefaultInitializationForImplementers();

    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();

    // 사용자 InitState를 직접 넘겨준다
    static const TArray<FGameplayTag> StateChain = 
    { 
        InitTags.InitState_Spawned, 
        InitTags.InitState_DataAvailable, 
        InitTags.InitState_DataInitialized, 
        InitTags.InitState_GameplayReady 
    };

    // 계속 상태가 가능할때까지 한번 업데이트 한다
    // 업데이트가 모두 완료되었다면, 내 상태로 자연스럽게 한 단계 올라간다(코드 참고, CurrentState)
    ContinueInitStateChain(StateChain);
}

void US1PawnHandler::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
    // OnAbilitySystemInitialized에 UObject가 바인딩되어 있지 않으면 추가(Uniqueness)
    if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
    {
        OnAbilitySystemInitialized.Add(Delegate);
    }

    // 이미 ASC가 설정되어있으면, Delegate에 추가하는게 아닌 바로 호출(이미 초기화 되어있기 때문)
    if (AbilitySystemComponent)
    {
        Delegate.Execute();
    }
}

void US1PawnHandler::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
    if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
    {
        OnAbilitySystemUninitialized.Add(Delegate);
    }
}
