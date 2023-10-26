// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/TenchuEnvironmentBase.h"
#include "TakeCoverBox.generated.h"

class UBoxComponent;
class ATenchuCharacter;
class USceneComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATakeCoverBox : public ATenchuEnvironmentBase
{
	GENERATED_BODY()
	
public:
	ATakeCoverBox();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = Interact)
	USceneComponent* PlayerLocation;

protected:
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	ATenchuCharacter* TenchuPlayer;
};