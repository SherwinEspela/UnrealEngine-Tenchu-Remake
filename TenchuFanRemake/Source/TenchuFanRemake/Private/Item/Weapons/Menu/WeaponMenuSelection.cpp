// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Item/Weapons/Menu/WeaponMenuSelection.h"
#include "Animation/Menu/Weapon/WeaponAnimInstance.h"

void AWeaponMenuSelection::BeginPlay()
{
	Super::BeginPlay();

	WeaponAnimInstance = Cast<UWeaponAnimInstance>(Mesh->GetAnimInstance());
}

void AWeaponMenuSelection::PlayConfirmSelectAnim()
{
	if (WeaponAnimInstance)
	{
		WeaponAnimInstance->SetConfirmedSelected();
	}
}
