// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/TenchuEnvironmentBase.h"
#include "Interface/IPlayerDetector.h"
#include "Wall.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API AWall : public ATenchuEnvironmentBase
{
	GENERATED_BODY()

public:
	AWall();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collider)
	TObjectPtr<UBoxComponent> Collider;

protected:
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
