// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(ContinueBtn)
	{
		// Bind our function to the button on clicked event
		ContinueBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnContinueClick);
	}

	if(RestartBtn)
	{
		// Bind our function to the button on clicked event
		RestartBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClick);
	}
}

void UPauseMenuWidget::OnContinueClick()
{
	if(const UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, false);
		RemoveFromParent();
	}
}

void UPauseMenuWidget::OnRestartClick()
{
	if(const UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}
