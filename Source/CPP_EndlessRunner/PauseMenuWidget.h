// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"


class UButton;


UCLASS()
class CPP_ENDLESSRUNNER_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ContinueBtn;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
    UButton* RestartBtn;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContinueClick();

	UFUNCTION()
	void OnRestartClick();
};
