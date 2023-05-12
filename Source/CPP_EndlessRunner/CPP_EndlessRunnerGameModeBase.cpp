// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPP_EndlessRunnerGameModeBase.h"

#include "FloorTile.h"
#include "GameHudWidget.h"
#include "Kismet/GameplayStatics.h"


void ACPP_EndlessRunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	GameHud = Cast<UGameHudWidget>(CreateWidget(GetWorld(), GameHudWidgetClass));
	check(GameHud);
	GameHud->InitHud(this);
	GameHud->AddToViewport();

	CurrentLivesCount = MaxLives;
	
	CreateInitialFloorTiles();
}


void ACPP_EndlessRunnerGameModeBase::CreateInitialFloorTiles()
{
	// Spawn a tile for the 1st time
	// Get the default arrows' "Y" value, and store them in the array
	if(const AFloorTile* Tile = AddFloorTile(false))
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	// // Print them just for debugging
	// for(float Val : LaneSwitchValues)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	// }

	AddFloorTile(false);
	AddFloorTile(false);
	
	// Loop by the number of tiles
	for(int i = 0; i < NumInitialFloorTiles; i++)
	{
		// Create them
		AddFloorTile(true);
	}
}

const AFloorTile* ACPP_EndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	// Check if the world exists
	if(UWorld* World = GetWorld())
	{
		// Check if we already spawned a tile of class type "FloorTile"
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if(Tile)
		{
			FloorTiles.Add(Tile);
			if(bSpawnItems)
			{
				Tile->SpawnItems();
			}
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}

void ACPP_EndlessRunnerGameModeBase::AddCoin()
{
	TotalCoins++;
	OnCoinsCountChanged.Broadcast(TotalCoins);
}

void ACPP_EndlessRunnerGameModeBase::PlayerDied()
{
	CurrentLivesCount--;
	OnLivesCountChanged.Broadcast(CurrentLivesCount);

	if(CurrentLivesCount > 0)
	{
		// Loop over all floor tiles and call DestroyFloorTile
		for (const auto Tile : FloorTiles)
		{
			Tile->DestroyFloorTile();
		}
		
		// Empty array
		FloorTiles.Empty();

		// reset NextSpawnPoint to it's initial value
		NextSpawnPoint = FTransform();
		
		// Create our initial floor tiles
		CreateInitialFloorTiles();
		
		// Broadcast level reset event
		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void ACPP_EndlessRunnerGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}

void ACPP_EndlessRunnerGameModeBase::GameOver()
{
	if(IsValid(GameOverWidgetClass))
	{
		if(UUserWidget* Widget = CreateWidget(GetWorld(), GameOverWidgetClass))
		{
			Widget->AddToViewport();
		}
	}
}

