
// Copyright (c) S1

#include "S1CameraComponent.h"
#include "S1CameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(S1CameraComponent)

US1CameraComponent::US1CameraComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CameraModeStack(nullptr)
{

}

void US1CameraComponent::OnRegister()
{
    Super::OnRegister();
    if (!CameraModeStack)
    {
        // 초기화가 딱히 필요없는 객체로 NewObject로 할당
        CameraModeStack = NewObject<US1CameraModeStack>(this);
    }

}

// 매 틱 업데이트 되는 함수
void US1CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
    check(CameraModeStack);
    
    // PlayerComponent에서 PawnData의 DefaultCameraMode를 가져온다
    // 이후 CameraMode에 추가하여, CameraMode 블렌딜을 준비한다
    UpdateCameraModes();

    // EvaluateStack은 CameraModeStack에 있는 CameraMode를 업데이트(+블렌딩)한다
    // 이후 CameraModeStack에서 Bottom-Top까지 업데이트된 CameraMode들을 Lerp해준다
    // 이에 대한 결과는 CameraModeView에 캐싱된다
    FS1CameraModeView CameraModeView;
    CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

    if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
    {
        if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
        {
            // 캐릭터의 회전을 적용시켜준다
            PC->SetControlRotation(CameraModeView.ControlRotation);
        }
    }

    // 카메라의 Location, Rotation을 업데이트 시켜준다
    SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

    // FOV 업데이트
    FieldOfView = CameraModeView.FieldOfView;

    /*
     *  ControlRotation vs Rotation
     *  - ControlRotation : PC가 조정하는 Pawn의 Rotation 적용값
     *  - Rotation : Camera에 반영하는 Rotation
     */

    // FMinimalViewInfo를 업데이트
    //  - CameraComponent의 변화 사항을 반영해서 최종 렌더링까지 반영
    DesiredView.Location = CameraModeView.Location;
    DesiredView.Rotation = CameraModeView.Rotation;
    DesiredView.FOV = CameraModeView.FieldOfView;
    DesiredView.OrthoWidth = OrthoWidth;
    DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
    DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
    DesiredView.AspectRatio = AspectRatio;
    DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
    DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
    DesiredView.ProjectionMode = ProjectionMode;
    DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
    if (PostProcessBlendWeight > 0.f)
    {
        DesiredView.PostProcessSettings = PostProcessSettings;
    }


}

void US1CameraComponent::UpdateCameraModes()
{
    check(CameraModeStack);

    // DetermineCameraModeDelegate는 CameraMode Class를 반환한다:
    // - 해당 Delegate는 PlayerComponent의 멤버 함수로 바인딩되어 있다
    if (DetermineCameraModeDelegate.IsBound())
    {
        if (TSubclassOf<US1CameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
        {
            CameraModeStack->PushCameraMode(CameraMode);
        }
    }
}


