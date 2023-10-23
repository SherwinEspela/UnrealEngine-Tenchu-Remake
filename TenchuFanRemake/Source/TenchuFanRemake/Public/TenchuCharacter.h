// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenchuCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;

UCLASS()
class TENCHUFANREMAKE_API ATenchuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATenchuCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	virtual void Jump() override;
	void ToggleCrouch();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

private:
	/* Private Functions */
	void AttachSword();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

	FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeed;
};
