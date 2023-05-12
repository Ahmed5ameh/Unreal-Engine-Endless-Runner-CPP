// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(MainMenuBtn)
	{
		// Bind our function to the button on clicked event
		MainMenuBtn->OnClicked.AddDynamic(this, &UGameOverWidget::OnMainMenuClick);
	}

	if(RestartBtn)
	{
		// Bind our function to the button on clicked event
		RestartBtn->OnClicked.AddDynamic(this, &UGameOverWidget::OnRestartClick);
	}
}

void UGameOverWidget::OnMainMenuClick()
{
	if(const UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, TEXT("MainMenu"));
	}
}

void UGameOverWidget::OnRestartClick()
{
	if(const UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}
