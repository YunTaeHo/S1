
// Copyright (c) S1

#include "CircumferenceMarkerWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CircumferenceMarkerWidget)

UCircumfenceMarkerWidget::UCircumfenceMarkerWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UCircumfenceMarkerWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    MyMarkerWidget->SetRadius(Radius);
    MyMarkerWidget->SetMarkerList(MarkerList);
}

TSharedRef<SWidget> UCircumfenceMarkerWidget::RebuildWidget()
{
    MyMarkerWidget = SNew(SCircumferenceMarkerWidget)
        .MarkerBrush(&MarkerImage)
        .Radius(this->Radius)
        .MarkerList(this->MarkerList);
    return MyMarkerWidget.ToSharedRef();
}

void UCircumfenceMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyMarkerWidget.Reset();
}


