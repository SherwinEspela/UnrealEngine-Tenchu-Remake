// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TenchuCSAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttachKatanaToHandNotifiedSignature);

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UTenchuCSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleAttachKatanaToHandNotification();

public:
	FORCEINLINE void SetConfirmedSelected() { bIsConfirmedSelected = true; }
	FORCEINLINE void SetSelected(bool Value) { bIsSelected = Value; }

public:
	FOnAttachKatanaToHandNotifiedSignature OnAttachKatanaToHandNotified;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Selected state")
	bool bIsSelected;

	UPROPERTY(BlueprintReadOnly, Category = "Selected state")
	bool bIsConfirmedSelected = false;
};
