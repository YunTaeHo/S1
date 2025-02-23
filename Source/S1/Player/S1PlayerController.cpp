
// Copyright (c) S1

#include "S1PlayerController.h"
#include "S1PlayerState.h"
#include "Camera/S1PlayerCameraManager.h"
#include "AbilitySystem/S1AbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1PlayerController)

// 기본 생성자
AS1PlayerController::AS1PlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PlayerCameraManagerClass = AS1PlayerCameraManager::StaticClass();
}

// Stack 방식으로 키를 모두 체크한 후 PostProcessInput를 호출 
void AS1PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    // PostProcessInput전에 후킹을 걸어 Input을 미리 처리해주자
    if (US1AbilitySystemComponent* S1ASC = GetS1AbilitySystemComponent())
    {
        S1ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
    }

    Super::PostProcessInput(DeltaTime, bGamePaused);
}

AS1PlayerState* AS1PlayerController::GetS1PlayerState() const
{
    // NullAllowed는 Null 반환을 의도할 경우 유용하기 때문에 사용
    return CastChecked<AS1PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

US1AbilitySystemComponent* AS1PlayerController::GetS1AbilitySystemComponent() const
{
    const AS1PlayerState* S1PS = GetS1PlayerState();
    return (S1PS ? S1PS->GetS1AbilitySystemComponent() : nullptr);
}

AS1PlayerCameraManager* AS1PlayerController::GetS1PlayerCameManager() const
{
    if (AS1PlayerCameraManager* CamManager = Cast<AS1PlayerCameraManager>(PlayerCameraManager))
    {
        return CamManager;
    }

    return nullptr;
}


