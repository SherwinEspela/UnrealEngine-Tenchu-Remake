// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TenchuAnimInstance.h"
#include "TenchuCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTenchuAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ATenchuCharacter* Character = Cast<ATenchuCharacter>(TryGetPawnOwner());
	if (Character) MovementComponent = Character->GetCharacterMovement();
}

void UTenchuAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		MovementSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	}
}
