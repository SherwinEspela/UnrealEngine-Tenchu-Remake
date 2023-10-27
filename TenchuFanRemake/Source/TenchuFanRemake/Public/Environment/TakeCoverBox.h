// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/TenchuEnvironmentBase.h"
#include "CustomEnums.h"
#include "Interface/InteractableInterface.h"
#include "TakeCoverBox.generated.h"

class UBoxComponent;
class ATenchuCharacter;
class USceneComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATakeCoverBox : public ATenchuEnvironmentBase, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	ATakeCoverBox();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = Interact)
	USceneComponent* PlayerLocation;

	UPROPERTY(EditAnywhere, Category = Interact)
	bool bIsStanding = true;

	UPROPERTY(EditAnywhere, Category = Interact)
	bool bIsFacingLeft = true;

	virtual void Interact() override;
	virtual EInteractableType GetInteractableType() override;

protected:
	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* IndicatorWidget;

private:
	ATenchuCharacter* TenchuPlayer;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EInteractableType InteractableType = EInteractableType::EIT_TakeCoverBox;
};
