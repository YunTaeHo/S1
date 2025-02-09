// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ClientUserWidget.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class S1_API UClientUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	AActor* GetOwner() { return Owner; }
	void SetOwner(AActor* InOwner) { Owner = InOwner; }

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Owner;

};
