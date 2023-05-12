// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	if(MainMenuWidgetClass)
	{
		if(UUserWidget* Widget = CreateWidget(GetWorld(), MainMenuWidgetClass))
		{
			Widget->AddToViewport();
		}
	}
}
