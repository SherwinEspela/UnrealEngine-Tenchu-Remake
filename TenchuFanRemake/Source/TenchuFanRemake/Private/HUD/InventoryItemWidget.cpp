// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::DisplayIndicator(bool Display) const
{
	if (Display)
	{
		ImageIndicator->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ImageIndicator->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryItemWidget::ShouldDisplayQuantityLabel(bool Display) const
{
	if (Display)
	{
		TextQuantity->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		TextQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryItemWidget::SetQuatityValue(int Value)
{
	TextQuantity->SetText(FText::FromString(FString::FromInt(Value)));
	CurrentQuantity = Value;
}

void UInventoryItemWidget::SetSelected(bool Selected)
{
	DisplayIndicator(Selected);
	float Scale = Selected ? 1.0f : 0.75f;
	FVector2D RenderScale(Scale, Scale);
	ImageItem->SetRenderScale(RenderScale);
	if (Selected)
	{
		OnSelected();
	}
	else {
		OnUnselected();
	}

	bIsSelected = Selected;
}
