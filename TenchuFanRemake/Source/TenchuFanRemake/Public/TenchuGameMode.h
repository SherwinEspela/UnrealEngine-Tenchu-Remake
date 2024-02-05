// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TenchuGameMode.generated.h"

class ARikimaruCharacter;
class ATenchuEnemyCharacter;
class UEnemyDetectorWidget;
class ATenchuHUD;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Debugging)
	bool bIsDebugging = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GetAllEnemies();

	UPROPERTY(EditAnywhere)
	int PercentDistanceOffset = 30;

private:
	ARikimaruCharacter* Player;
	TArray<ATenchuEnemyCharacter*> Enemies;
	ATenchuEnemyCharacter* ClosestEnemy;

	UFUNCTION(BlueprintCallable)
	void HandleEnemyDied(ATenchuEnemyCharacter* Enemy);

	double ClosestDistanceTotal;
	
	
	void FindClosestEnemy();

	UEnemyDetectorWidget* EnemyDetectorWidget;
	void SetEnemyDetectorWidget(UEnemyDetectorWidget* Widget);

	ATenchuHUD* TenchuHUD;
};
