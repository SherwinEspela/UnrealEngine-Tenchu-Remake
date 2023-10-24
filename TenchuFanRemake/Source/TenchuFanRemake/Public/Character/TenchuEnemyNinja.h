// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TenchuBaseCharacter.h"
#include "TenchuEnemyNinja.generated.h"

class USphereComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuEnemyNinja : public ATenchuBaseCharacter
{
	GENERATED_BODY()
public:
	ATenchuEnemyNinja();

	virtual void BeginPlay() override;

protected:
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;*/

	UPROPERTY(EditAnywhere)
	UWidgetComponent* EnemyCloseWidget;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
