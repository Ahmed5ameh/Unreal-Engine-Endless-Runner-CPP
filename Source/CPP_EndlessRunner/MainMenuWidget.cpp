// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(StartBtn)
	{
		// Bind our function to the button on clicked event
		StartBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClick);
	}

	if(QuitBtn)
	{
		// Bind our function to the button on clicked event
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClick);
	}
}

void UMainMenuWidget::OnStartClick()
{
	if(const UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, TEXT("MainLevel"));
	}
}

void UMainMenuWidget::OnQuitClick()
{
	if(const UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
	}
}
