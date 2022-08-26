// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJ13_API ACustomHUD : public AHUD
{
	GENERATED_BODY()

public:
	class UClass * hudWidgetClass;
	class UUserWidget * hudWidget;
	virtual void BeginPlay() override;
	ACustomHUD();

};
