// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TenchuHUD.h"
#include "HUD/EnemyDetectorWidget.h"
#include "HUD/InventoryWidget.h"

void ATenchuHUD::BeginPlay()
{
	UWorld* World = GetWorld();

	EnemyDetectorWidget = CreateWidget<UEnemyDetectorWidget>(World, EnemyDetectorWidgetClass);
	EnemyDetectorWidget->AddToViewport();

	InventoryWidget = CreateWidget<UInventoryWidget>(World, InventoryWidgetClass);
	InventoryWidget->AddToViewport();
}
