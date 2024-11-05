// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Menu/PlayerSelectionActor.h"
#include "PlayerSelectionRikimaru.generated.h"

class AWeaponMenuSelection;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API APlayerSelectionRikimaru : public APlayerSelectionActor
{
	GENERATED_BODY()

public:
	APlayerSelectionRikimaru();

	virtual void ConfirmSelect() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleAttachToHandleNotification();

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeaponMenuSelection> KatanaWeaponClass;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeaponMenuSelection> KatanaCoverWeaponClass;

private:
	AWeaponMenuSelection* Katana;
	AWeaponMenuSelection* KatanaCover;
};
