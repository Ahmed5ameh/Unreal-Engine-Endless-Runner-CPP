// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "CPP_EndlessRunnerGameModeBase.h"
#include "Obstacle.h"
#include "RunCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Coin.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterLane"));
	CenterLane->SetupAttachment(SceneComponent);
	
	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	DestroyAfterTime = 1.25f;
}


// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	// Get world game mode
	// Cast it to our game's game mode
	// Assign it to the variable
	// Check if the cast succeeded
	GameMode = Cast<ACPP_EndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(GameMode);

	// Bind overlap functions
	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerOverlap);
}


void AFloorTile::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Cast "OtherActor" to a "RunCharacter" class
	// If it succeeded, it means that we've been hit by the player
	if(ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		GameMode->AddFloorTile(true);

		// Access world timer manager, and set a timer, like a cooldown or smth
		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, DestroyAfterTime, false);
		//GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AFloorTile::DestroyFloorTile, 2.0f, false);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if(DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	for(auto Child : ChildActors)
	{
		if(IsValid(Child))
		{
			Child->Destroy();
		}
	}
	ChildActors.Empty();
	
	GameMode->RemoveTile(this);
	
	// Destroy the floor after the amount of time "DestroyTimerHandle" has passed 
	this->Destroy();
}

void AFloorTile::SpawnItems()
{
	if(IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinClass))
	{
		int32 NumBigs = 0;
		SpawnLaneItem(CenterLane, NumBigs);
		SpawnLaneItem(LeftLane, NumBigs);
		SpawnLaneItem(RightLane, NumBigs);
	}
}

void AFloorTile::SpawnLaneItem(const UArrowComponent* Lane, int32& BigObstaclesCount)
{
	// Get random value 0 : 1
	const float RandVal = FMath::FRandRange(0.0f, 1.0f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Get lane position: (right, left, mid) lanes
	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	// If "RandomVal" is between 0.1 : 0.3
	if(UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent1, SpawnPercent2, true, true))
	{
		// Spawn Small Obstacle: so it's a 20% chance to be a small obstacle in each lane
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
		ChildActors.Add(Obstacle);
	}
	
	// If "RandomVal" is between 0.3 : 0.5
	else if(UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent2, SpawnPercent3, true, true))
	{
		// To prevent 3 big obstacles being in the same lane
		if(BigObstaclesCount <= 2)
		{
			// Spawn Big Obstacle: so it's a 20% chance to be a big obstacle in each lane
			if(AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParameters))
			{
				BigObstaclesCount++;
				ChildActors.Add(Obstacle);
			}
			else
			{
				// Spawn a small obstacle
				GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
				ChildActors.Add(Obstacle);
			}
		}
	}

	else if(UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent3, 1.0f, true, true))
	{
		// Spawn Coin: so it's a 50% chance to be a coin in each lane
		ACoin* Coin = GetWorld()->SpawnActor<ACoin>(CoinClass, SpawnLocation, SpawnParameters);
		ChildActors.Add(Coin);
	}
}
