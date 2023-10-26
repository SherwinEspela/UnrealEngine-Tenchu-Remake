// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomEnums.h"
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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	ETenchuPlayerStates TenchuPlayerState = ETenchuPlayerStates::EPS_Idle;

public:
	FORCEINLINE float GetWalkSpeed() const { return WalkSpeed; }
	FORCEINLINE void SetEnemyToStealthAttack(ATenchuEnemyCharacter* NewEnemy) { EnemyToStealthAttack = NewEnemy; }
	FORCEINLINE void RemoveEnemyToStealthAttack() { EnemyToStealthAttack = nullptr; }
	FORCEINLINE bool CanStealthAttack() const { return EnemyToStealthAttack != nullptr; }
	FORCEINLINE void SetActorToInteract(AActor* NewActor) { ActorToInteract = NewActor; }
	FORCEINLINE void RemoveActorToInteract() { ActorToInteract = nullptr; }
	FORCEINLINE bool CanInteract() { return ActorToInteract != nullptr; }

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

	UPROPERTY(EditAnywhere, Category = Movement)
	TObjectPtr<UAnimMontage> MontageJump;

	UPROPERTY(EditAnywhere, Category = Stealth)
	TObjectPtr<UAnimMontage> MontageTakeCover;

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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* ActorToInteract;

	UPROPERTY(EditAnywhere, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	float TakeCoverInterpSpeed = 10.f;

private:
	/* Private Functions */
	void AttachSword();
	void PlayStealthAttackAnimation();
};
