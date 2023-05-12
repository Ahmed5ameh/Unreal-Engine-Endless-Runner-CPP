// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

class APlayerStart;

UCLASS()
class CPP_ENDLESSRUNNER_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunCharacter();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")
	UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")
	USoundBase* DeathSound;

	UPROPERTY()
	FTimerHandle RestartHandle;

	UPROPERTY()
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MoveDownImpulse = -1000.0f;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Lane")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category="Lane")
	void ChangeLaneUpdate(float Value);
	
	UFUNCTION(BlueprintCallable, Category="Lane")
	void ChangeLaneFinished();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION()
	void AddCoin() const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"));
	class USpringArmComponent* CameraArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"));
	class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	class ACPP_EndlessRunnerGameModeBase* GameMode;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerStart* PlayerStart;
	
	UFUNCTION() void MoveLeft();
	UFUNCTION() void MoveRight();
	UFUNCTION() void MoveDown();
	UFUNCTION() void OnDeath();
	UFUNCTION()	void ResetLevel();

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
