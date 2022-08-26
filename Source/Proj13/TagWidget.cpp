// Fill out your copyright notice in the Description page of Project Settings.


#include "TagWidget.h"

#include "Components/TextBlock.h"



void UTagWidget::SetDisplayText(FString Text) {
	Text = FString("");
	DisplayText->SetText(FText::FromString(Text));
}

void UTagWidget::ShowPlayerNetRole(APawn *InPawn) {
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole) {
	case ROLE_Authority:
		Role = FString("Authority"); break;
	case ROLE_AutonomousProxy:
		Role = FString("Auto"); break;
	case ROLE_SimulatedProxy:
		Role = FString("Simu"); break;
	case ROLE_None:
		Role = FString("None"); break;
	}
	SetDisplayText(Role);
}