// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TenchuHUD.generated.h"

class UEnemyDetectorWidget;
class UInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDetectorWidgetAddedSignature, UEnemyDetectorWidget*, EnemyDetectorWidget);

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API ATenchuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FEnemyDetectorWidgetAddedSignature OnEnemyDetectorWidgetAdded;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEnemyDetectorWidget> EnemyDetectorWidgetClass;

	UPROPERTY()
	TObjectPtr<UEnemyDetectorWidget> EnemyDetectorWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

public:
	FORCEINLINE UEnemyDetectorWidget* GetEnemyDetectorWidget() const { return EnemyDetectorWidget; }
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
};
