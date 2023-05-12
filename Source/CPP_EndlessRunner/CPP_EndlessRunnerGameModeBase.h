// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_EndlessRunnerGameModeBase.generated.h"

class AFloorTile;
class UUserWidget;
class UGameHudWidget;

// Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChanged, int32, CoinsCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesCountChanged, int32, LivesCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelReset);

UCLASS()
class CPP_ENDLESSRUNNER_API ACPP_EndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<AFloorTile> FloorTileClass;
	
	UPROPERTY(EditAnywhere, Category="Config")
	int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	TArray<float> LaneSwitchValues;

	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<UUserWidget> GameHudWidgetClass;

	UPROPERTY(VisibleAnywhere)
	int32 TotalCoins = 0;
	
	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UGameHudWidget* GameHud;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnCoinsCountChanged OnCoinsCountChanged;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentLivesCount = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxLives = 3;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnCoinsCountChanged OnLivesCountChanged;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	TArray<AFloorTile*> FloorTiles;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Delegates")
	FOnLevelReset OnLevelReset;

	UPROPERTY(EditAnywhere, Category="Config")
	TSubclassOf<UUserWidget> GameOverWidgetClass;


	
	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();
	
	UFUNCTION(BlueprintCallable)
	const AFloorTile* AddFloorTile(const bool bSpawnItems);

	UFUNCTION(BlueprintCallable)
	void AddCoin();

	UFUNCTION(BlueprintCallable)
	void PlayerDied();

	UFUNCTION(BlueprintCallable)
	void RemoveTile(AFloorTile* Tile);

	UFUNCTION(BlueprintCallable)
	void GameOver();


protected:
	virtual void BeginPlay() override;
};
