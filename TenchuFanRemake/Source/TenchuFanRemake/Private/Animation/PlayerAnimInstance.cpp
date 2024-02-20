// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Character/RikimaruCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ARikimaruCharacter>(TryGetPawnOwner());
	if (PlayerCharacter) MovementComponent = PlayerCharacter->GetCharacterMovement();

	bJumpStarted = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter)
	{
		FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	}
	
	if (MovementComponent)
	{
		MovementSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		//UE_LOG(LogTemp, Warning, TEXT("MovementSpeed = %f"), MovementSpeed);
	}
}

void UPlayerAnimInstance::SetJumpTypeByMovementOffsetYaw()
{
	bIsJumpFlipping = true;
	bJumpStarted = true;
	float FrontThreshold = 30.f;
	float BackThreshold = 150.f;

	if (MovementOffsetYaw < FrontThreshold && MovementOffsetYaw > -FrontThreshold)
	{
		JumpType = EJumpType::EJT_FrontFlip;
	}
	else if (MovementOffsetYaw >= BackThreshold || MovementOffsetYaw <= -BackThreshold) {
		JumpType = EJumpType::EJT_BackFlip;
	}
	else {
		bIsJumpFlipping = false;
		bJumpStarted = false;
	}
}
