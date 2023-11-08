// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "HUD/InventoryItemWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemGrapplingHook->SetSelected(true);
	ItemRation->SetSelected(false);
	ItemShuriken->SetSelected(false);

	ItemRation->SetQuatityValue(3);
	ItemShuriken->SetQuatityValue(7);

	Items.Add(ItemGrapplingHook);
	Items.Add(ItemRation);
	Items.Add(ItemShuriken);

	CurrentIndex = 0;
	MaxIndex = Items.Num() - 1;
	CurrentItem = ItemGrapplingHook;
}

void UInventoryWidget::SelectRight()
{
	--CurrentIndex;
	if (CurrentIndex < MIN_INDEX) CurrentIndex = MaxIndex;
	UpdateSelection();
}

void UInventoryWidget::SelectLeft()
{
	++CurrentIndex;
	if (CurrentIndex > MaxIndex) CurrentIndex = MIN_INDEX;
	UpdateSelection();
}

void UInventoryWidget::UpdateSelection()
{
	CurrentItem->SetSelected(false);
	CurrentItem = Items[CurrentIndex];
	CurrentItem->SetSelected(true);
}
