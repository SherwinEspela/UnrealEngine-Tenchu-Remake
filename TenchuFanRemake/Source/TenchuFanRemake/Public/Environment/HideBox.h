// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomEnums.h"
#include "Interface/InteractableInterface.h"
#include "HideBox.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class TENCHUFANREMAKE_API AHideBox : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AHideBox();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = Interact)
	USceneComponent* PlayerLocation;

	UPROPERTY(EditAnywhere, Category = Interact)
	UWidgetComponent* IndicatorWidget;

	UPROPERTY(EditAnywhere, Category = Interact)
	bool bIsStanding = true;

	UPROPERTY(EditAnywhere, Category = Interact)
	bool bIsFacingLeft = true;

	virtual void Interact() override;
	virtual EInteractableType GetInteractableType() override;

protected:
	virtual void BeginPlay() override;

};
