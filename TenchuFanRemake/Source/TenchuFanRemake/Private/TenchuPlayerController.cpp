// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Character/RikimaruCharacter.h"
#include "HUD/TenchuHUD.h"
#include "HUD/InventoryWidget.h"

void ATenchuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARikimaruCharacter>(GetPawn());

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContextPlayer, 0);

	TenchuHUD = Cast<ATenchuHUD>(GetHUD());
}

void ATenchuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputActionMovement, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Move);
	EnhancedInputComponent->BindAction(InputActionLookAround, ETriggerEvent::Triggered, this, &ATenchuPlayerController::LookAround);
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Jump);
	EnhancedInputComponent->BindAction(InputActionInteract, ETriggerEvent::Triggered, this, &ATenchuPlayerController::Interact);
	EnhancedInputComponent->BindAction(InputActionYButton, ETriggerEvent::Triggered, this, &ATenchuPlayerController::SwordInteract);
	EnhancedInputComponent->BindAction(InputActionR2Button, ETriggerEvent::Started, this, &ATenchuPlayerController::Crouch);
	EnhancedInputComponent->BindAction(InputActionR2Button, ETriggerEvent::Completed, this, &ATenchuPlayerController::UnCrouch);
	EnhancedInputComponent->BindAction(InputActionDPadRight, ETriggerEvent::Triggered, this, &ATenchuPlayerController::DpadRightClicked);
	EnhancedInputComponent->BindAction(InputActionDPadLeft, ETriggerEvent::Triggered, this, &ATenchuPlayerController::DpadLeftClicked);
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
	//PlayerCharacter->PlayerJump();
}

void ATenchuPlayerController::Crouch()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->Crouch();
	}
}

void ATenchuPlayerController::UnCrouch()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->UnCrouch();
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

void ATenchuPlayerController::DpadRightClicked()
{
	TenchuHUD->GetInventoryWidget()->SelectRight();
}

void ATenchuPlayerController::DpadLeftClicked()
{
	TenchuHUD->GetInventoryWidget()->SelectLeft();
}
