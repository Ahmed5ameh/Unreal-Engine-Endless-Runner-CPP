// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHudWidget.generated.h"

class UTextBlock;
class ACPP_EndlessRunnerGameModeBase;
class UButton;

/**
 * 
 */
UCLASS()
class CPP_ENDLESSRUNNER_API UGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitHud(ACPP_EndlessRunnerGameModeBase* GameMode);

	UFUNCTION(BlueprintCallable)
	void SetCoinsCount(int32 Count);

	UFUNCTION(BlueprintCallable)
	void SetLivesCount(int32 Count);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CoinsCount;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* PauseBtn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* LivesCount;
	
	UFUNCTION()
	void OnPauseClick();

	virtual void NativeConstruct() override;
};
