// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/TenchuEnvironmentBase.h"
#include "Interface/InteractableInterface.h"
#include "ClimableWall.generated.h"

class UBoxComponent;
class ARikimaruCharacter;
class USceneComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API AClimableWall : public ATenchuEnvironmentBase, public IInteractableInterface
{
	GENERATED_BODY()
public:
	AClimableWall();
	virtual void BeginPlay() override;
	virtual EInteractableType GetInteractableType() override;

	void SetWarpTargetTransform(FTransform Value);
	void SetWarpTargetPosition(FVector Value);

public:
	FORCEINLINE FTransform GetWarpTargetTransform() const { return WarpTarget->GetComponentTransform(); }

protected:
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* WarpTarget;

private:
	ARikimaruCharacter* Rikimaru;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EInteractableType InteractableType = EInteractableType::EIT_ClimableWall;
};
