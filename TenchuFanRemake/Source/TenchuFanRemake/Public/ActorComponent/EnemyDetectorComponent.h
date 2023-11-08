// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyDetectorComponent.generated.h"

class ATenchuCharacter;
class ATenchuEnemyCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TENCHUFANREMAKE_API UEnemyDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyDetectorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
	bool bIsDebugging = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GetAllEnemies();
		
private:
	ATenchuCharacter* Player;
	TArray<ATenchuEnemyCharacter*> Enemies;
	ATenchuEnemyCharacter* ClosestEnemy;

	double ClosestDistanceTotal;
	const int PercentDistanceOffset = 30;

	void FindClosestEnemy();

};
