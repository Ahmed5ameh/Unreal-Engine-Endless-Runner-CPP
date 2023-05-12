// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHudWidget.h"
#include "CPP_EndlessRunnerGameModeBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(PauseBtn)
	{
		// Bind our function to the button on clicked event
		PauseBtn->OnClicked.AddDynamic(this, &UGameHudWidget::OnPauseClick);
	}
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		this);
}


void UGameHudWidget::InitHud(ACPP_EndlessRunnerGameModeBase* GameMode)
{
	if(GameMode)
	{
		// Init coins count to be zero
		CoinsCount->SetText(FText::AsNumber(0));
		LivesCount->SetText(FText::AsNumber(GameMode->MaxLives));
		

		// Bind Events
		GameMode->OnCoinsCountChanged.AddDynamic(this, &UGameHudWidget::SetCoinsCount);
		GameMode->OnLivesCountChanged.AddDynamic(this, &UGameHudWidget::SetLivesCount);
	}
}

void UGameHudWidget::SetCoinsCount(const int32 Count)
{
	CoinsCount->SetText(FText::AsNumber(Count));
}

void UGameHudWidget::SetLivesCount(const int32 Count)
{
	LivesCount->SetText(FText::AsNumber(Count));
}

void UGameHudWidget::OnPauseClick()
{
	if(IsValid(PauseMenuWidgetClass))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		if(UUserWidget* Widget = CreateWidget(GetWorld(), PauseMenuWidgetClass))
		{
			Widget->AddToViewport();
		}
	}
}


