// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TenchuBaseCharacter.h"
#include "CustomEnums.h"
#include "Interface/InteractableInterface.h"
#include "TenchuEnemyCharacter.generated.h"

class USphereComponent;
class UWidgetComponent;
class UAnimMontage;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuEnemyCharacter : public ATenchuBaseCharacter, public IInteractableInterface
{
	GENERATED_BODY()
public:
	ATenchuEnemyCharacter();
	virtual void BeginPlay() override;
	void StealthDeath(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword);

	UPROPERTY(BlueprintReadWrite)
	EEnemyStates EnemyStates = EEnemyStates::ES_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyDeathPose DeathPose = EEnemyDeathPose::EDP_Pose1;

	UFUNCTION()
	FVector GetPlayerStealthKillLocation(bool bWithSword);

	UFUNCTION()
	FRotator GetPlayerStealthKillRotation();

	virtual void Interact() override;
	virtual EInteractableType GetInteractableType();

	FORCEINLINE bool GetIsStealthAttackFromBack() const { return bIsStealthAttackFromBack; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* EnemyCloseWidget;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthDeath;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthDeathBackNoSword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USceneComponent> PlayerSteathKillPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USceneComponent> PlayerSteathKillPositionFront;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USceneComponent> PlayerSteathKillPositionBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UCameraComponent> StealthKillCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Combat)
	TObjectPtr<USpringArmComponent> StealthKillCameraBoom;

	UPROPERTY(EditAnywhere, Category = "Stealth Positions")
	FVector PlayerSteathKillBackLocationWithSword;

	UPROPERTY(EditAnywhere, Category = "Stealth Positions")
	FVector PlayerSteathKillBackLocationNoSword;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EInteractableType InteractableType = EInteractableType::EIT_Enemy;

	void GetStealthPosition(AActor* Player);
	bool bIsStealthAttackFromBack = true;
};
