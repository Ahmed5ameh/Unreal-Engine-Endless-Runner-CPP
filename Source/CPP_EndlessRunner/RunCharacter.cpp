// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"

#include "CPP_EndlessRunnerGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Spring Arm */
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.0f;
	CameraArm->SocketOffset = FVector(0,0,100.0f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	/* Camera */
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
}


// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get world game mode
	// Cast it to our game's game mode
	// Assign it to the variable
	// Check if the cast succeeded
	GameMode = Cast<ACPP_EndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(GameMode);

	// Bind our function to OnLevelResetEvent at the GameMode
	GameMode->OnLevelReset.AddDynamic(this, &ARunCharacter::ResetLevel);

	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	AddMovementInput(ControlRot.Vector());
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding movement functions to input actions
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARunCharacter::MoveDown);

	// Jumping is implemented using built in jumping functions in UE's default Character class
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}



void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0,2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0,2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	const static FVector Impulse = FVector(0,0,MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(GameMode->LaneSwitchValues[CurrentLane], GameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::Die()
{
	if(bIsDead)	return;
	
	const FVector Location = GetActorLocation();
	if(const UWorld* World = GetWorld())
	{
		bIsDead = true;

		// Disable player input
		DisableInput(nullptr);
	
		if(DeathParticleSystem)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
		}

		if(DeathSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
		}
		GetMesh()->SetVisibility(false);
		World->GetTimerManager().SetTimer(RestartHandle, this, &ARunCharacter::OnDeath, 1.0f);
	}
}

void ARunCharacter::OnDeath()
{
	// Clear timer
	if(RestartHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartHandle);
	}

	GameMode->PlayerDied();

	// Execute a restart command
	//UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

void ARunCharacter::AddCoin() const
{
	GameMode->AddCoin();
}

void ARunCharacter::ResetLevel()
{
	bIsDead = false;
	EnableInput(nullptr);
	GetMesh()->SetVisibility(true);
	if(PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}
}