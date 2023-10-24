// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "TenchuCharacter.h"

ATenchuPlayerController::ATenchuPlayerController()
{
}

void ATenchuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContextPlayer, 0);

	PlayerCharacter = Cast<ATenchuCharacter>(GetPawn());
}

void ATenchuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputActionMovement, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Move);
	EnhancedInputComponent->BindAction(InputActionLookAround, ETriggerEvent::Triggered, this, &ATenchuPlayerController::LookAround);
	//EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Jump);
	EnhancedInputComponent->BindAction(InputActionToggleCrouch, ETriggerEvent::Triggered, this, &ATenchuPlayerController::ToggleCrouch);
	
	if (InputActionStealthAttack)
	{
		EnhancedInputComponent->BindAction(InputActionStealthAttack, ETriggerEvent::Triggered, this, &ATenchuPlayerController::PlayStealthAttack);
	}
}

void ATenchuPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void ATenchuPlayerController::LookAround(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	GetPawn()->AddControllerYawInput(LookAxisVector.X);
	GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
}

void ATenchuPlayerController::Jump()
{
	//ATenchuCharacter* PlayerCharacter = Cast<ATenchuCharacter>(GetPawn());
	//PlayerCharacter->Jump();
}

void ATenchuPlayerController::ToggleCrouch()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ToggleCrouch();
	}
}

void ATenchuPlayerController::PlayStealthAttack()
{
	if (PlayerCharacter && PlayerCharacter->CanStealthAttack())
	{
		PlayerCharacter->StealthAttack();
	}
}
