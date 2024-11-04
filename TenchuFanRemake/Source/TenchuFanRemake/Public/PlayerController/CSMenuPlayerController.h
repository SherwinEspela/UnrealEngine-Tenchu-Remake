// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSMenuPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;
class APlayerSelectionActor;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ACSMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeftSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRightSelected();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void SelectLeft();
	void SelectRight();

protected:
	// Player Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Input")
	UInputAction* InputActionRight;

protected:
	APlayerSelectionActor* PlayerSelectionRikimaru;
	APlayerSelectionActor* PlayerSelectionAyame;
	bool bIsLeft = true;
};
