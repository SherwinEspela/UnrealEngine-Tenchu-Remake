// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TenchuBaseCharacter.h"
#include "CustomEnums.h"
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
class TENCHUFANREMAKE_API ATenchuEnemyCharacter : public ATenchuBaseCharacter
{
	GENERATED_BODY()
public:
	ATenchuEnemyCharacter();
	virtual void BeginPlay() override;
	void StealthDeath(FName SectionName, EEnemyDeathPose NewDeathPose);

	UPROPERTY(BlueprintReadWrite)
	EEnemyStates EnemyStates = EEnemyStates::ES_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyDeathPose DeathPose = EEnemyDeathPose::EDP_Pose1;

	UFUNCTION()
	FVector GetPlayerStealthKillLocation();

	UFUNCTION()
	FRotator GetPlayerStealthKillRotation();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* EnemyCloseWidget;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> MontageStealthDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USceneComponent> PlayerSteathKillPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UCameraComponent> StealthKillCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Combat)
	TObjectPtr<USpringArmComponent> StealthKillCameraBoom;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
