// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TakeCoverComponent.generated.h"

class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TENCHUFANREMAKE_API UTakeCoverComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTakeCoverComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = Interact)
	USceneComponent* PlayerLocation;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
