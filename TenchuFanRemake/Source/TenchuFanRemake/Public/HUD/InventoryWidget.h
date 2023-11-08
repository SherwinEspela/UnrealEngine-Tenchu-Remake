// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryItemWidget;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SelectRight();

	UFUNCTION(BlueprintCallable)
	void SelectLeft();

protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UInventoryItemWidget* ItemGrapplingHook;

	UPROPERTY(meta = (BindWidget))
	UInventoryItemWidget* ItemRation;

	UPROPERTY(meta = (BindWidget))
	UInventoryItemWidget* ItemShuriken;

private:
	TArray<UInventoryItemWidget*> Items;

	int CurrentIndex;
	const int MIN_INDEX = 0;
	int MaxIndex;

	UInventoryItemWidget* CurrentItem;

	void UpdateSelection();
};
