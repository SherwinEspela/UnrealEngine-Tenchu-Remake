// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TenchuBaseCharacter.h"
#include "GameFramework/Character.h"
#include "CustomEnums.h"
#include "Interface/InteractableInterface.h"
#include "RikimaruCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class ATenchuEnemyCharacter;
class UAnimMontage;
class UAnimInstance;
class ATakeCoverBox;
class AActionCam;
class UPlayerAnimInstance;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ARikimaruCharacter : public ATenchuBaseCharacter
{
	GENERATED_BODY()
public:
	ARikimaruCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void Move(FVector2D MovementVector, FVector ForwardDirection, FVector RightDirection);
	void LookAround(FVector2D LookAxisVector);
	void PlayerJump();
	void JumpFlip();
	void ClimbLedge();
	void ClimbLedgeTop();
	void StealthAttack();
	void TakeCover();
	void Interact();
	void SwordInteract();
	void ToggleCrouch();
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	ETenchuPlayerStates TenchuPlayerState = ETenchuPlayerStates::EPS_Idle;

	UPROPERTY(BlueprintReadOnly)
	ETakeCoverType TakeCoverType = ETakeCoverType::ETC_FacingLeft;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStandingWhenTakingCover = true;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTakingCoverFacingLeft = true;

	UFUNCTION(BlueprintCallable)
	void HandleSwordUnsheatingCompleted();

	UFUNCTION(BlueprintCallable)
	void HandleSwordSheatingCompleted();

public:
	FORCEINLINE void SetActorToInteract(IInteractableInterface* NewInteractable) { Interactable = NewInteractable; }
	FORCEINLINE void RemoveActorToInteract() { Interactable = nullptr; }
	FORCEINLINE bool CanInteract() { return Interactable != nullptr; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetWalkingSpeed() const { return WalkingSpeed; }

protected:
	bool IsWallTraced(FHitResult& OutHitResult);
	bool IsWallHeightTraced(FHitResult& OutHitResult);
	bool CanTraceWall(FVector Start, FVector End, FHitResult& OutHit);

protected:
	virtual void BeginPlay() override;
	void TakeCoverBoxInterp(float DeltaTime);
	AWeapon* Katana;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bIsStealthDebugEnabled = false;

	UPROPERTY(EditAnywhere, Category = Debug)
	int StealthSectionIndexToDebug = 1;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bIsStealthRandomized = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CrouchingSpeed = 140.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkingSpeed = 227.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RunningSpeed = 340.f;

protected:
	// Climbing System
	UFUNCTION(BlueprintImplementableEvent)
	void OnClimbTransformWarpTargetAdded();

	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float ClimbStateWallSurfaceOffset = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	float ClimbStateWallHeightOffset = 206.f;

	UPROPERTY(BlueprintReadOnly, Category = Climbing)
	FTransform ClimbTransformWarpTarget;

	UPROPERTY(EditDefaultsOnly, Category = Climbing)
	TObjectPtr<UAnimMontage> MontageClimbLedge;

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

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthKillBackNoSword;

	UPROPERTY(EditAnywhere, Category = Movement)
	TObjectPtr<UAnimMontage> MontageJump;

	UPROPERTY(EditAnywhere, Category = Stealth)
	TObjectPtr<UAnimMontage> MontageTakeCover;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> MontageSwordInteraction;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

	UPROPERTY(EditDefaultsOnly)
	float CrouchSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TSubclassOf<AActionCam> ActionCamClass;

private:
	AActionCam* ActionCam;
	FVector CrouchEyeOffset;
	int CurrentStealthIndex = 1;
	UPlayerAnimInstance* PlayerAnimInstance;
	ATakeCoverBox* TakeCoverBox;
	bool bTakeCoverBoxInterpCompleted = false;

private:
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
	void AttachSword();
	void PlayStealthAttackAnimation(FName SectionName, int SectionIndex);
	void AttachSwordToSocket(FName SocketName);
};
