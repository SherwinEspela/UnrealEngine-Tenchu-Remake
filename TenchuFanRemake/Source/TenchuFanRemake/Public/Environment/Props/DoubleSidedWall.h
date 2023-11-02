// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/Props/Props.h"
#include "DoubleSidedWall.generated.h"

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ADoubleSidedWall : public AProps
{
	GENERATED_BODY()
public:
	ADoubleSidedWall();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MirrorWallMesh;
};
