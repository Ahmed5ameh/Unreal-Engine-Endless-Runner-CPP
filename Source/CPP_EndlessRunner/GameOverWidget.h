// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;

UCLASS()
class CPP_ENDLESSRUNNER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* MainMenuBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* RestartBtn;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMainMenuClick();

	UFUNCTION()
	void OnRestartClick();
};
