// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void URunAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if(Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if(IsValid(Pawn))
	{
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Pawn && MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}
}
