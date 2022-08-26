// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJ13_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ScoreText;

	void SetDisplayText(FString Text);

	UFUNCTION(BlueprintCallable)
	void ShowScore(int InScore);
};
