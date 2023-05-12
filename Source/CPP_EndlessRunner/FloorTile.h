// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class USceneComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ACPP_EndlessRunnerGameModeBase;
class AObstacle;
class ACoin;

UCLASS()
class CPP_ENDLESSRUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* RightLane;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* LeftLane;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<ACoin> CoinClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	float SpawnPercent1 = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	float SpawnPercent3 = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ChildActors;
	
	FORCEINLINE const FTransform GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}

	UFUNCTION(BlueprintCallable)
	void SpawnItems();

	UFUNCTION(BlueprintCallable)
	void DestroyFloorTile();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	ACPP_EndlessRunnerGameModeBase* GameMode;

	UPROPERTY()
	FTimerHandle DestroyHandle;

	UPROPERTY(EditAnywhere, Category="Timers")
	float DestroyAfterTime;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnLaneItem(const UArrowComponent* Lane, int32& BigObstaclesCount);

};
