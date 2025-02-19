
// Copyright (c) S1

#include "S1PlayerComponent.h"
#include "S1LogChannel.h"
#include "S1PawnHandler.h"
#include "S1GameplayTags.h"
#include "PlayerMappableInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/S1PlayerState.h"
#include "Player/S1PlayerController.h"
#include "Camera/S1CameraComponent.h"
#include "Input/S1MappableConfigPair.h"
#include "Input/S1InputComponent.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include "Character/S1PawnData.h"
#include "Character/S1Character.h"
#include "GameFramework/Character.h"
#include "Equipment/S1QuickBarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerComponent)

/** FeatureName 정의 : static member variable 초기화 */
const FName US1PlayerComponent::NAME_ActorFeatureName("PlayerComponent");

/** InputConfig의 GameFeatureAction 활성화 ExtensionEvent 이름 */
const FName US1PlayerComponent::NAME_BindInputsNow("BindInputsNow");


US1PlayerComponent::US1PlayerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;
}


TSubclassOf<US1CameraMode> US1PlayerComponent::DetermineCameraMode() const
{
    if (AbilityCameraMode)
    {
        return AbilityCameraMode;
    }

    const APawn* Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return nullptr;
    }

    if (US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
    {
        if (const US1PawnData* PawnData = PawnHandler->GetPawnData<US1PawnData>())
        {
            return PawnData->DefaultCameraMode;
        }
    }

    return nullptr;
}



void US1PlayerComponent::OnRegister()
{
    Super::OnRegister();

    // 올바른 Actor에 등록되었는지 확인:
    {
        if (!GetPawn<APawn>())
        {
            UE_LOG(LogS1, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
        }
    }

    RegisterInitStateFeature();
}

void US1PlayerComponent::BeginPlay()
{
    Super::BeginPlay();

    // PawnHandler에 대해서 (PawnHandler Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
    BindOnActorInitStateChanged(US1PawnHandler::NAME_ActorFeatureName, FGameplayTag(), false);

    // InitState_Spawned로 초기화
    ensure(TryToChangeInitState(FS1GameplayTags::Get().InitState_Spawned));

    // ForceUpdate 진행
    CheckDefaultInitialization();
}

void US1PlayerComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    UnregisterInitStateFeature();
    Super::EndPlay(EndPlayReason);
}

void US1PlayerComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
    const FS1GameplayTags InitTags = FS1GameplayTags::Get();
    if (Params.FeatureName == US1PawnHandler::NAME_ActorFeatureName)
    {
        // US1PawnHandler가 DataInitialized라면?
        if (Params.FeatureState == InitTags.InitState_DataInitialized)
        {
            // 나도 DataInitialized로 전환하고 싶다
            CheckDefaultInitialization();
        }
    }
}

bool US1PlayerComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
    check(Manager);

    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();
    APawn* Pawn = GetPawn<APawn>();
    AS1PlayerState* S1PS = GetPlayerState<AS1PlayerState>();

    // Spawned 초기화
    if(!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
    {
        if (Pawn)
        {
            return true;
        }
    }

    // Spawned -> DataAvailable
    if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
    {
        // PawnData가 설정되어있지 않다면 InitState를 작동시키지 않는다
        if (!S1PS)
        {
            return false;
        }

        return true;
    }

    // DataAvailable -> DataInitialized
    if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
    {
        // US1PawnHandler가 DataInitialized될 때까지 기다린 (== 모든 Feature Component가 DataAvailable 인 상태
        return S1PS && Manager->HasFeatureReachedInitState(Pawn, US1PawnHandler::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
    }

    // DataInitialized -> GameplayReady
    if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
    {
        return true;
    }

    return false;
}

void US1PlayerComponent::CheckDefaultInitialization()
{
    const FS1GameplayTags& InitTags = FS1GameplayTags::Get();

    // PawnHandler와 같은 코드
    static const TArray<FGameplayTag> StateChain =
    {
        InitTags.InitState_Spawned,
        InitTags.InitState_DataAvailable,
        InitTags.InitState_DataInitialized,
        InitTags.InitState_GameplayReady
    };

    ContinueInitStateChain(StateChain);
}

void US1PlayerComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
    const FS1GameplayTags InitTags = FS1GameplayTags::Get();

    // DataAvailable -> DataInitialized
    if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
    {
        APawn* Pawn = GetPawn<APawn>();
        AS1PlayerState* S1PS = GetPlayerState<AS1PlayerState>();
        if (!ensure(Pawn && S1PS))
        {
            return;
        }

        const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
        const US1PawnData* PawnData = nullptr;
        if (US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
        {
            PawnData = PawnHandler->GetPawnData<US1PawnData>();

            // DataInitialized 단계까지 오면, Pawn이 Controller에 Possess되어 준비된 상태이다:
            // InitializeActorInfo 호출로 AvatarActor 재설정이 필요하다
            PawnHandler->InitializeAbilitySystem(S1PS->GetS1AbilitySystemComponent(), S1PS);
        }

        if (bIsLocallyControlled && PawnData)
        {
            // 현재 Character에 Attach된 CameraComponent를 찾음
            if (US1CameraComponent* CameraComponent = US1CameraComponent::FindCameraComponent(Pawn))
            {
                CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
            }
        }

        if (AS1PlayerController* S1PC = GetController<AS1PlayerController>())
        {
            if (Pawn->InputComponent != nullptr)
            {
                InitializePlayerInput(Pawn->InputComponent);
            }
        }
    }
}

void US1PlayerComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    const APawn* Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    // LocalPlayer를 가져오기 위해
    const APlayerController* PC = GetController<APlayerController>();
    check(PC);

    // EnhancedInputLocalPlayerSubsystem 가져오기 위해
    const ULocalPlayer* LP = PC->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem< UEnhancedInputLocalPlayerSubsystem>();
    check(Subsystem);

    // UEnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다
    Subsystem->ClearAllMappings();

    // PawnHandler -> PawnData -> InputConfig 존재 유무 판단
    if (const US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
    {
        if (const US1PawnData* PawnData = PawnHandler->GetPawnData<US1PawnData>())
        {
            if (const US1InputConfig* InputConfig = PawnData->InputConfig)
            {
                const FS1GameplayTags& GameplayTags = FS1GameplayTags::Get();

                // PlayerComponent가 가지고 있는 Input Mapping Context를 순회하며, UEnhancedInputLocalPlayerSubsystem에 추가한다
                for (const FS1MappableConfigPair& Pair : DefaultInputConfig)
                {
                    if (Pair.bShouldActivateAutomatically)
                    {
                        FModifyContextOptions Options = {};
                        Options.bIgnoreAllPressedKeysUntilRelease = false;
                        
                        Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
                    }
                }

                US1InputComponent* S1IC = CastChecked<US1InputComponent>(PlayerInputComponent);
                {
                    // InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킨다
                    // - 바인딩 이후, Input 이벤트에 따라 멤버 함수가 트리거된다
                    {
                        TArray<uint32> BindHandles;
                        S1IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
                    }
                    S1IC->BindNativeActions(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
                    S1IC->BindNativeActions(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
                    S1IC->BindNativeActions(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Started, this, &ThisClass::Input_Sprint, false);
                    S1IC->BindNativeActions(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Completed, this, &ThisClass::Input_Sprint, false);
                    S1IC->BindNativeActions(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Started, this, &ThisClass::Input_Crouch, false);
                }
            }
        }
    }

    // GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void US1PlayerComponent::Input_Move(const FInputActionValue& InputActionValue)
{
    APawn* Pawn = GetPawn<APawn>();
    AController* Controller = Pawn ? Pawn->GetController() : nullptr;

    if (Controller)
    {
        const FVector2D Value = InputActionValue.Get<FVector2D>();
        const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

        if (Value.X != 0.f)
        {
            // Left/Right -> X 값에 들어있음
            // MovementDirection은 현재 카메라의 RightVector를 의미함 (World-Space)
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

            Pawn->AddMovementInput(MovementDirection, Value.X);
        }

        if (Value.Y != 0.f)
        {
            // Left/Right와 마찬가지로 Forward/Backword를 적용
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
            Pawn->AddMovementInput(MovementDirection, Value.Y);
        }
    }
}

void US1PlayerComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
    APawn* Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    const FVector2D Value = InputActionValue.Get<FVector2D>();
    if (Value.X != 0.f)
    {
        // 애니메이션 제어
        if (AS1Character* Character = Cast<AS1Character>(Pawn))
        {
            Character->SetTurnRate(Value.X);
        }

        // X : Yaw값
        // Camera에 대해 Yaw 값 적용

        Pawn->AddControllerYawInput(Value.X);
    }

    if (Value.Y != 0.f)
    {
        // Y : Pitch값
        double AimInversionValue = -Value.Y;
        Pawn->AddControllerPitchInput(AimInversionValue);
    }
}

void US1PlayerComponent::Input_Sprint(const FInputActionValue& InputActionValue)
{
    if (APawn* Pawn = GetPawn<APawn>())
    {
        if (AS1Character* S1Character = Cast<AS1Character>(Pawn))
        {
            S1Character->SetSprinting();
        }
    }
}

void US1PlayerComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
    if (APawn* Pawn = GetPawn<APawn>())
    {
        if (AS1Character* S1Character = Cast<AS1Character>(Pawn))
        {
            if (S1Character->IsFalling())
            {
                return;
            }

            if (S1Character->IsCrouch())
            {
                FVector Location = S1Character->GetActorLocation();
                FRotator Rotation = S1Character->GetActorRotation();
                TArray<AActor*> ActorToIgnore;
                FHitResult Hit;
                float CrouchSize = 100.f;

                // 위에 장애물이 있어 Crouch를 풀 수 없는 상황이라면 풀지 말아야함
                if (!UKismetSystemLibrary::LineTraceSingle
                (
                    GetWorld(),
                    Location,
                    Location + FRotationMatrix(Rotation).GetScaledAxis(EAxis::Z) * CrouchSize,
                    ETraceTypeQuery::TraceTypeQuery1,
                    false,
                    ActorToIgnore,
                    EDrawDebugTrace::ForDuration,
                    Hit,
                    true
                ))
                {
                    S1Character->UnCrouch();
                    S1Character->SetCrouched(false);
                }
            }
            else
            {
                S1Character->Crouch();
                S1Character->SetCrouched(true);
            }

        }
    }

}

void US1PlayerComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (const APawn* Pawn = GetPawn<APawn>())
    {
        if (const US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
        {
            if (US1AbilitySystemComponent* S1ASC = PawnHandler->GetS1AbilitySystemComponent())
            {
                S1ASC->AbilityInputTagPressed(InputTag);
            }
        }
    }
}

void US1PlayerComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (const APawn* Pawn = GetPawn<APawn>())
    {
        if (const US1PawnHandler* PawnHandler = US1PawnHandler::FindPawnHandler(Pawn))
        {
            if (US1AbilitySystemComponent* S1ASC = PawnHandler->GetS1AbilitySystemComponent())
            {
                S1ASC->AbilityInputTagReleased(InputTag);
            }
        }
    }
}

void US1PlayerComponent::SetAbilityCameraMode(TSubclassOf<US1CameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
    if (CameraMode)
    {
        AbilityCameraMode = CameraMode;
        AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
    }
}

void US1PlayerComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
    if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
    {
        AbilityCameraMode = nullptr;
        AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
    }
}

