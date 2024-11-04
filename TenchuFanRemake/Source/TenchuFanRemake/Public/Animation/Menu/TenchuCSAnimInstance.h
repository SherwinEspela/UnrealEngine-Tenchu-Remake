// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TenchuCSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UTenchuCSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetSelected(bool Value) { bIsSelected = Value; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Selected state")
	bool bIsSelected;
};
