// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomEnums.h"
#include "Interface/InteractableInterface.h"
#include "TenchuCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class ATenchuEnemyCharacter;
class UAnimMontage;
class UAnimInstance;
class ATakeCoverBox;

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

	void PlayerJump();
	void ToggleCrouch();
	void StealthAttack();
	void TakeCover();
	void Interact();
	void SwordInteract();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	ETenchuPlayerStates TenchuPlayerState = ETenchuPlayerStates::EPS_Idle;

	UPROPERTY(BlueprintReadOnly)
	ETakeCoverType TakeCoverType = ETakeCoverType::ETC_FacingLeft;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStandingWhenTakingCover = true;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTakingCoverFacingLeft = true;

public:
	FORCEINLINE float GetWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE void SetActorToInteract(IInteractableInterface* NewInteractable) { Interactable = NewInteractable; }
	FORCEINLINE void RemoveActorToInteract() { Interactable = nullptr; }
	FORCEINLINE bool CanInteract() { return Interactable != nullptr; }

protected:
	virtual void BeginPlay() override;

	void TakeCoverBoxInterp(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthAttacks;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthAttacksFront;

	UPROPERTY(EditAnywhere, Category = Movement)
	TObjectPtr<UAnimMontage> MontageJump;

	UPROPERTY(EditAnywhere, Category = Stealth)
	TObjectPtr<UAnimMontage> MontageTakeCover;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> MontageSwordInteraction;

private:
	/* Private Members / Properties */

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeed;

	float WalkSpeed;
	FVector CrouchEyeOffset;

	UAnimInstance* AnimInstance;
	ATakeCoverBox* TakeCoverBox;

	bool bTakeCoverBoxInterpCompleted = false;

	/*
	* The Enemy to which the Player
	* can perform the Stealth attack to
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ATenchuEnemyCharacter> EnemyToStealthAttack;

	IInteractableInterface* Interactable;

	UPROPERTY(EditAnywhere, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	float TakeCoverInterpSpeed = 10.f;

	UPROPERTY(BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"));
	bool bIsSwordEquipped = true;

private:
	/* Private Functions */
	void AttachSword();
	void PlayStealthAttackAnimation();
};
