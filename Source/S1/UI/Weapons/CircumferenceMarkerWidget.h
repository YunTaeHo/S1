
// Copyright (c) S1

#pragma once

#include "Components/Widget.h"
#include "SCircumferenceMarkerWidget.h"
#include "CircumferenceMarkerWidget.generated.h"

UCLASS()
class S1_API UCircumfenceMarkerWidget : public UWidget
{
    GENERATED_BODY()

public:
    UCircumfenceMarkerWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /*
     * UWidget`s Interfaces
     */
    /** SWidget과 UWidget간 데이크 싱크를 맞추기 위한 메서드 */
    virtual void SynchronizeProperties() override;
    /** Widget 재생성*/
    virtual TSharedRef<SWidget> RebuildWidget() override;

    /*
     * UVisual Interfaces
     */
    /** 해당 UWidget은 MyMarkerWidget의 SWidget을 가지고 있으므로 잘 해제해야 한다 */
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    TArray<FCircumferenceMarkerEntry> MarkerList;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    float Radius = 40.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    FSlateBrush MarkerImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    uint8 bReticleCornerOutsideSpreadRadius : 1;

    /** UMG의 CircumferenceMarkerWidget에 대응되는 SWidget */
    TSharedPtr<SCircumferenceMarkerWidget> MyMarkerWidget;



}; 
