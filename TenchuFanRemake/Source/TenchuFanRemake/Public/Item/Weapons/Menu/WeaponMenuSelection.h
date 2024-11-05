// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapons/Weapon.h"
#include "WeaponMenuSelection.generated.h"

class UWeaponAnimInstance;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API AWeaponMenuSelection : public AWeapon
{
	GENERATED_BODY()
	
public:
	void PlayConfirmSelectAnim();

protected:
	virtual void BeginPlay() override;

protected:
	UWeaponAnimInstance* WeaponAnimInstance;
};
