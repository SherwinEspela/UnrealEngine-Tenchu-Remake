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

void UEnemyDetectorWidget::DisplayIndicators(bool Display)
{
	if (Display)
	{
		TextRange->SetVisibility(ESlateVisibility::Visible);
		ImageGuess->SetVisibility(ESlateVisibility::Visible);
		ImageGuessHighlight->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		TextRange->SetVisibility(ESlateVisibility::Hidden);
		ImageGuess->SetVisibility(ESlateVisibility::Hidden);
		ImageGuessHighlight->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEnemyDetectorWidget::UpdateDisplayFromRangeValue(int Value)
{
	FString RangeValueString = FString::FromInt(Value);
	TextRange->SetText(FText::FromString(RangeValueString));

	float Scale = Value / 100.f;
	Scale = FMath::Clamp(Scale, 0.4f, 0.7f);
	FVector2D RenderScale(Scale, Scale);
	ImageGuess->SetRenderScale(RenderScale);
	ImageGuessHighlight->SetRenderScale(RenderScale * 1.35f);
}
