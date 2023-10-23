// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenchuBaseCharacter.generated.h"

UCLASS()
class TENCHUFANREMAKE_API ATenchuBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATenchuBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
