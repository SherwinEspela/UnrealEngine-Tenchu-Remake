// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetConfirmedSelected() { bIsConfirmedSelected = true; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation state")
	bool bIsConfirmedSelected = false;
};
