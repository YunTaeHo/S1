
// Copyright (c) S1

#pragma once

#include "Containers/Array.h"
#include "Math/Vector2D.h"
#include "Misc/Attribute.h"
#include "Rendering/SlateRenderTransform.h"
#include "Styling/CoreStyle.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"
#include "SCircumferenceMarkerWidget.generated.h"

USTRUCT(BlueprintType)
struct FCircumferenceMarkerEntry
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ForceUnits = deg))
    float PositionAngle = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ForceUnits = deg))
    float ImageRotationAngle = 0.f;

};

class SCircumferenceMarkerWidget : public SLeafWidget
{
    SLATE_BEGIN_ARGS(SCircumferenceMarkerWidget)
        : _MarkerBrush(FCoreStyle::Get().GetBrush("Throbber.CircleChunk"))
        , _Radius(48.f)
        {}
        /** 각 마커에 대한 Brush(Box or Image) */
        SLATE_ARGUMENT(const FSlateBrush*, MarkerBrush)
        /** 마커 리스트 */
        SLATE_ARGUMENT(TArray<FCircumferenceMarkerEntry>, MarkerList)
        /** 원의 둘레 */
        SLATE_ATTRIBUTE(float, Radius)
        /** 마커의 색깔 혹은 Opacity(투명도) */
        SLATE_ATTRIBUTE(FSlateColor, ColorAndOpaicty)
    SLATE_END_ARGS()

public:
    SCircumferenceMarkerWidget();


    /** FArgument는 앞서 선언한 SLATE_BEGIN_ARGS에 의해 결정된다(매크로 확인*/
    void Construct(const FArguments& InArgs);

    void SetRadius(float NewRadius);
    void SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList);
    FSlateRenderTransform GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker, const float BaseRadius, const float HUDScale) const;


    /*
     * SWidget Interfaces
     */
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
    virtual FVector2D ComputeDesiredSize(float) const override;
    virtual bool ComputeVolatility() const override { return true;  }

    /*
     * 앞서 정의한 SALTE_ARGS와 비슷하지만 하나씩 기억해두자
     */

    /** circumference(원주율, 원형태)의 MarkerBrush */
    const FSlateBrush* MarkerBrush;

    /** reticle 중심으로 외각 코너의 각 아이콘들(마커들) */
    TArray<FCircumferenceMarkerEntry> MarkerList;

    /** reticle 원의 Radius */
    TAttribute<float> Radius;

    /** marker의 color & Opacity */
    TAttribute<FSlateColor> ColorAndOpaicty;
    bool bColorAndOpacitySet;

    /** 각 마커를 원 밖으로 뺄지 말지 확인 */
    uint8 bReticleCornerOutsideSpreadRadius : 1;

}; 
