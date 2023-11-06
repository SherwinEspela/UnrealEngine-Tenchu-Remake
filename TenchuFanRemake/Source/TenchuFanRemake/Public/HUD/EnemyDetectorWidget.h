// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyDetectorWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UEnemyDetectorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTextRangeValue(FString Value);
	void DisplayIndicators(bool Display);
	void UpdateDisplayFromRangeValue(int Value);

protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextRange;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageGuess;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageGuessHighlight;
};
