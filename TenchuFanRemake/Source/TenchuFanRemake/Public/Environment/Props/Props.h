// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Props.generated.h"

class ATakeCoverBox;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API AProps : public AActor
{
	GENERATED_BODY()
public:
	AProps();

protected:

	UPROPERTY(EditAnywhere, Category = "Take Cover Box")
	ATakeCoverBox* Box1;
};
