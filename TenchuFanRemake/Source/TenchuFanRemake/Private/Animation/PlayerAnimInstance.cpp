// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "TenchuCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ACharacter* Character = Cast<ATenchuCharacter>(TryGetPawnOwner());
	if (Character) MovementComponent = Character->GetCharacterMovement();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		MovementSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
	}
}
