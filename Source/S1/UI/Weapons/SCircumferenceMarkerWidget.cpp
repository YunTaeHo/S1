
// Copyright (c) S1

#include "SCircumferenceMarkerWidget.h"
#include "Engine/UserInterfaceSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SCircumferenceMarkerWidget)

SCircumferenceMarkerWidget::SCircumferenceMarkerWidget()
{

}

void SCircumferenceMarkerWidget::Construct(const FArguments& InArgs)
{
    MarkerBrush = InArgs._MarkerBrush;
    MarkerList = InArgs._MarkerList;
    Radius = InArgs._Radius;
    bColorAndOpacitySet = InArgs._ColorAndOpaicty.IsSet();
    ColorAndOpaicty = InArgs._ColorAndOpaicty;
}

void SCircumferenceMarkerWidget::SetRadius(float NewRadius)
{
    if (Radius.IsBound() || (Radius.Get() != NewRadius))
    {
        Radius = NewRadius;
        Invalidate(EInvalidateWidgetReason::Layout);
    }
}

void SCircumferenceMarkerWidget::SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList)
{
    MarkerList = NewMarkerList;
}

FSlateRenderTransform SCircumferenceMarkerWidget::GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker, const float BaseRadius, const float HUDScale) const
{
    float XRadius = BaseRadius;
    float YRadius = BaseRadius;
    if (bReticleCornerOutsideSpreadRadius)
    {
        // Image 사이즈의 반만큼 (Radius) 더해줘서 원 바깥에서 마커가 렌더링 되도록 한다
        XRadius += MarkerBrush->ImageSize.X * 0.5f;
        YRadius += MarkerBrush->ImageSize.Y * 0.5f;
    }

    // degree -> radian
    const float LocalRotationRadians = FMath::DegreesToRadians(Marker.ImageRotationAngle);
    const float PositionAngleRadians = FMath::DegreesToRadians(Marker.PositionAngle);

    FSlateRenderTransform RotateAboutOrigin(
        // RotateAboutOrigin은 ImageSize/2.0만큼 옮기고 회전시키고 다시 옮겨놓는다
        // - 중심좌표 기준으로 회전하고 다시 원위치 시키는 기능이다
        Concatenate(
            FVector2D(-MarkerBrush->ImageSize.X * 0.5f, -MarkerBrush->ImageSize.Y * 0.5f),
            FQuat2D(LocalRotationRadians),
            FVector2D(MarkerBrush->ImageSize.X * 0.5f, MarkerBrush->ImageSize.Y * 0.5f)
        )
    );


    // 회전한 Image를 circumference 방식으로 위치를 translation 시켜준다
    // - 회전행렬 공식을 이용해 회전시키고 Transform 좌표로 이동시켜준다
    return TransformCast<FSlateRenderTransform>(
        Concatenate(
            RotateAboutOrigin,
            FVector2D(XRadius * FMath::Sin(PositionAngleRadians) * HUDScale, -YRadius * FMath::Cos(PositionAngleRadians) * HUDScale)
        )
    );
}

int32 SCircumferenceMarkerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    // GetLocalPositionAtCoordinate(@d(0.f, 0.f)) == 중심좌표가 나온다
    const FVector2D LocalCenter = AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f));

    const bool bDrawMarkers = (MarkerList.Num() > 0) && (MarkerBrush != nullptr);
    if (bDrawMarkers)
    {
        const FLinearColor MarkerColor = bColorAndOpacitySet ?
            ColorAndOpaicty.Get().GetColor(InWidgetStyle) :
            (InWidgetStyle.GetColorAndOpacityTint() * MarkerBrush->GetTint(InWidgetStyle));

        // 투명도가 0이 아니라면(조금이라도 보인다면)
        if (MarkerColor.A > KINDA_SMALL_NUMBER)
        {
            const float BaseRadius = Radius.Get();
            const float ApplicationScale = GetDefault<UUserInterfaceSettings>()->ApplicationScale;

            // 계수 만큼 회전시켜서 크로스 헤어를 만들어준다
            for (const FCircumferenceMarkerEntry& Marker : MarkerList)
            {
                const FSlateRenderTransform MarkerTransform = GetMarkerRenderTransform(Marker, BaseRadius, ApplicationScale);
                const FPaintGeometry Geometry(
                    AllottedGeometry.ToPaintGeometry(MarkerBrush->ImageSize, FSlateLayoutTransform(LocalCenter - (MarkerBrush->ImageSize * 0.5)),
                    MarkerTransform,
                    FVector2D(0.f, 0.f))
                );

                FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geometry, MarkerBrush, DrawEffects, MarkerColor);
            }
        }
    }

    return LayerId;
}

FVector2D SCircumferenceMarkerWidget::ComputeDesiredSize(float) const
{
    // ComputeDesiredSize()는 Immediate 구조 UI 프레임워크에서는 매 프레임 불린다
    // Retained 방식 Qt (규격이 정해져있음 -> 렌더링 알아서)
    // Immediate IMGUI (유저가 알아서 렌더링을 하는 방식)
    check(MarkerBrush);
    const float SampledRadius = Radius.Get();
    return FVector2D((MarkerBrush->ImageSize.X + SampledRadius) * 2.f, (MarkerBrush->ImageSize.Y + SampledRadius) * 2.f);
}
