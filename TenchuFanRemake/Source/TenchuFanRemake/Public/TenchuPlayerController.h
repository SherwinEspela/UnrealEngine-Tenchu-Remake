// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TenchuPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ARikimaruCharacter;
class ATenchuHUD;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MovementSpeedMultiplier = 2.f;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputMappingContext> InputMappingContextPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> InputActionMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> InputActionLookAround;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> InputActionJump;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> InputActionToggleCrouch;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionInteract;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionYButton;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionR2Button;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionDPadRight;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionDPadLeft;

	/*UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionForwardJumpFlip;*/

	UPROPERTY();
	TObjectPtr<ARikimaruCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool bIsJumping = false;

private:
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Jump();
	void JumpFlip();
	void Crouch();
	void UnCrouch();
	void ToggleCrouch();
	void Interact();
	void SwordInteract();
	void DpadRightClicked();
	void DpadLeftClicked();

	TObjectPtr<ATenchuHUD> TenchuHUD;
};
