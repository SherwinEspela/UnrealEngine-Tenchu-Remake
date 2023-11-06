// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class USphereComponent;

UCLASS()
class TENCHUFANREMAKE_API AActionCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AActionCamera();
	virtual void Tick(float DeltaTime) override;

	void SetRandomRotation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> RotationControl;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;

protected:
	virtual void BeginPlay() override;
};
