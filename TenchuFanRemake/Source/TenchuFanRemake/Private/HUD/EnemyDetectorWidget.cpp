// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EnemyDetectorWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SlateWrapperTypes.h"

void UEnemyDetectorWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyDetectorWidget::SetTextRangeValue(FString Value)
{
	TextRange->SetText(FText::FromString(Value));
}

void UEnemyDetectorWidget::DisplayTextRange(bool Display)
{
	if (Display)
	{
		TextRange->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		TextRange->SetVisibility(ESlateVisibility::Hidden);
	}
}
