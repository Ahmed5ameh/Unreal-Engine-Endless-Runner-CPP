// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UButton;


UCLASS()
class CPP_ENDLESSRUNNER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* StartBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* QuitBtn;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartClick();

	UFUNCTION()
	void OnQuitClick();
};
