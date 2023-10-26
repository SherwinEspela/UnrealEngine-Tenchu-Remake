// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TenchuPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATenchuCharacter;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATenchuPlayerController();

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
	TObjectPtr<UInputAction> InputActionStealthAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputAction* InputActionTakeCover;

	UPROPERTY();
	TObjectPtr<ATenchuCharacter> PlayerCharacter;

private:
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Jump();
	void ToggleCrouch();
	void PlayStealthAttack();
	void TakeCover();
};
