// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

#include "Components/TextBlock.h"

void UScoreWidget::SetDisplayText(FString Text) {
	ScoreText->SetText(FText::FromString(Text));
}

void UScoreWidget::ShowScore(int InScore) {
	FString s = FString::Printf(TEXT("Score : %d"), InScore);
	SetDisplayText(s);
}