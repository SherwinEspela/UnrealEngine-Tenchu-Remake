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

	TurnInPlace();
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

void UPlayerAnimInstance::TurnInPlace()
{
	if (PlayerCharacter == nullptr) return;

	if (bIsClimbing)
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = PlayerCharacter->GetActorRotation().Yaw;
		const float YawDelta = CharacterYaw - CharacterYawLastFrame;
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);
		return;
	}

	if (MovementSpeed > 0)
	{
		RootYawOffset = 0.f;
		CharacterYaw = PlayerCharacter->GetActorRotation().Yaw;
		CharacterYawLastFrame = CharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
		//UE_LOG(LogTemp, Warning, TEXT("Is moving...."));
	}
	else {
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = PlayerCharacter->GetActorRotation().Yaw;
		const float YawDelta = CharacterYaw - CharacterYawLastFrame;
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);
		//UE_LOG(LogTemp, Warning, TEXT("Is not moving...."));
		//UE_LOG(LogTemp, Warning, TEXT("RootYawOffset ==== %f"), RootYawOffset);

		if (bIsClimbing) return;
		const float Turning = GetCurveValue(TEXT("Turning"));
		if (Turning > 0)
		{
			RotationCurveLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));
			const float DeltaRotation = RotationCurve - RotationCurveLastFrame;
			const bool IsTurningLeft = RootYawOffset > 0.f;
			IsTurningLeft ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;
			RootYawOffset = FMath::Clamp(RootYawOffset, -90.f, 90.f);
		}
	}
}

void UPlayerAnimInstance::ResetRootYawOffset()
{
	RootYawOffset = 0.f;
}
