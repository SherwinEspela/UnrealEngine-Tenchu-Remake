// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenchuCharacter.generated.h"

UCLASS()
class TENCHUFANREMAKE_API ATenchuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATenchuCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

};
