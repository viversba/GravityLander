// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay() {
	
	Super::BeginPlay();

	if (HUDOverlayAsset) {
		HUDOVerlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOVerlay->AddToViewport();
	HUDOVerlay->SetVisibility(ESlateVisibility::Visible);
}