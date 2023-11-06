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
class AAIController;
class ATenchuEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDiedSignature, ATenchuEnemyCharacter*, Enemy);

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
	virtual void Tick(float DeltaTime) override;
	
	void StealthDeathFront(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword);
	void StealthDeathBack(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword);

	UPROPERTY(BlueprintReadWrite)
	EEnemyStates EnemyState = EEnemyStates::ES_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyDeathPose DeathPose = EEnemyDeathPose::EDP_Pose1;

	UPROPERTY(BlueprintAssignable)
	FEnemyDiedSignature OnEnemyDied;

	UFUNCTION()
	FVector GetPlayerStealthKillLocation(FName SectionName, bool bWithSword);

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
	TObjectPtr<UAnimMontage> MontageStealthDeathFront;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthDeathBackNoSword;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy AI")
	TObjectPtr<UAnimMontage> MontageIdleTurn;

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

	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI")
	AAIController* EnemyAIController;

	UPROPERTY(EditInstanceOnly, Category = "Enemy AI")
	TArray<AActor*> NavigationWaypoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Enemy AI")
	bool bIsPatrolling;

	UPROPERTY(EditAnywhere, Category = "Enemy AI")
	float PatrolAcceptanceRadius = 120.f;

	UPROPERTY(BlueprintReadOnly)
	UAnimInstance* EnemyAnimInstance;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void HandleIdleTurningComplete();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EInteractableType InteractableType = EInteractableType::EIT_Enemy;

	void GetStealthPosition(AActor* Player);
	bool bIsStealthAttackFromBack = true;

	AActor* CurrentWayPoint;

	void ObserveIfWaypointReached();
	void SelectNextWaypoint();

	bool bIsWaypointReached = false;

	FTimerHandle PatrolTimer;
	void PatrolIdlingTimeFinished();

	void StealthDeath(EEnemyDeathPose NewDeathPose);
};
