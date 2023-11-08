// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* TextureItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsIndicatorDisplayed = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsQuantityLabelDisplayed = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSelected = false;

public:
	UFUNCTION(BlueprintCallable)
	void DisplayIndicator(bool Display) const;

	UFUNCTION(BlueprintCallable)
	void ShouldDisplayQuantityLabel(bool Display) const;

	UFUNCTION(BlueprintCallable)
	void SetQuatityValue(int Value);

	UFUNCTION(BlueprintCallable)
	void SetSelected(bool Selected);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelected();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnselected();

protected:
	void NativeConstruct() override;

private:
	bool bIsInitialized = false;
};
