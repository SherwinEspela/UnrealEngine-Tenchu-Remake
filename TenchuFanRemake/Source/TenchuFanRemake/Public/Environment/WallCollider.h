// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallCollider.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TENCHUFANREMAKE_API AWallCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	AWallCollider();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collider)
	TObjectPtr<UBoxComponent> Collider;

};
