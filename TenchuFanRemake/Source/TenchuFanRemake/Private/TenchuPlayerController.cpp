// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "TenchuCharacter.h"

void ATenchuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ATenchuCharacter>(GetPawn());

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContextPlayer, 0);
}

void ATenchuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputActionMovement, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Move);
	EnhancedInputComponent->BindAction(InputActionLookAround, ETriggerEvent::Triggered, this, &ATenchuPlayerController::LookAround);
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Jump);
	EnhancedInputComponent->BindAction(InputActionToggleCrouch, ETriggerEvent::Triggered, this, &ATenchuPlayerController::ToggleCrouch);
	EnhancedInputComponent->BindAction(InputActionInteract, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Interact);
	EnhancedInputComponent->BindAction(InputActionYButton, ETriggerEvent::Triggered, this, &ATenchuPlayerController::SwordInteract);
}

void ATenchuPlayerController::Move(const FInputActionValue& Value)
{
	if (PlayerCharacter->TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);

	PlayerCharacter->TenchuPlayerState = ETenchuPlayerStates::EPS_Moving;
}

void ATenchuPlayerController::LookAround(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	GetPawn()->AddControllerYawInput(LookAxisVector.X);
	GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
}

void ATenchuPlayerController::Jump()
{
	PlayerCharacter->PlayerJump();
}

void ATenchuPlayerController::ToggleCrouch()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->ToggleCrouch();
	}
}

void ATenchuPlayerController::Interact()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->Interact();
	}
}

void ATenchuPlayerController::SwordInteract()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SwordInteract();
	}
}
