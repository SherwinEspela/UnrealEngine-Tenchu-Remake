// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Character/Menu/PlayerSelectionRikimaru.h"
#include "Item/Weapons/Menu/WeaponMenuSelection.h"
#include "Animation/Menu/TenchuCSAnimInstance.h"

APlayerSelectionRikimaru::APlayerSelectionRikimaru()
{

}

void APlayerSelectionRikimaru::BeginPlay()
{
	Super::BeginPlay();

	if (KatanaWeaponClass && KatanaCoverWeaponClass)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

		Katana = GetWorld()->SpawnActor<AWeaponMenuSelection>(KatanaWeaponClass);
		Katana->AttachToComponent(PlayerSelectionMesh, TransformRules, FName("SWORDCASE"));

		KatanaCover = GetWorld()->SpawnActor<AWeaponMenuSelection>(KatanaCoverWeaponClass);
		KatanaCover->AttachToComponent(PlayerSelectionMesh, TransformRules, FName("SWORDCASE"));
	}

	if (PSAnimInstance)
	{
		PSAnimInstance->OnAttachKatanaToHandNotified.AddDynamic(this, &APlayerSelectionRikimaru::HandleAttachToHandleNotification);
	}
}

void APlayerSelectionRikimaru::ConfirmSelect()
{
	Super::ConfirmSelect();

	if (Katana)
	{
		Katana->PlayConfirmSelectAnim();
	}

	if (KatanaCover)
	{
		KatanaCover->PlayConfirmSelectAnim();
	}
}

void APlayerSelectionRikimaru::HandleAttachToHandleNotification()
{
	if (!Katana) return;
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	Katana->AttachToComponent(PlayerSelectionMesh, TransformRules, FName("WEAPON_R"));
}
