// Copyright 2024 Sherwin Espela. All rights reserved.


#include "PlayerController/CSMenuPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/Menu/PlayerSelectionActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CSMenuUserWidget.h"

#define NAME_RIKIMARU TEXT("Rikimaru")

void ACSMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	PlayerSubsystem->AddMappingContext(InputMappingContext, 0);

	TSubclassOf<APlayerSelectionActor> PlayerSelectionActorClass = APlayerSelectionActor::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, PlayerSelectionActorClass, OutActors);

	if (OutActors.Num() > 0)
	{
		for (auto FoundActor : OutActors) {
			if (FoundActor->GetName().ToLower().Contains(NAME_RIKIMARU))
			{
				PlayerSelectionRikimaru = Cast<APlayerSelectionActor>(FoundActor);
			}
			else {
				PlayerSelectionAyame = Cast<APlayerSelectionActor>(FoundActor);
			}
		}
	}

	bIsLeft = true;

	if (CSMenuUserWidgetClass)
	{
		CSMenuUserWidget = CreateWidget<UCSMenuUserWidget>(GetWorld(), CSMenuUserWidgetClass);
		CSMenuUserWidget->AddToViewport();
	}
}

void ACSMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputActionLeft, ETriggerEvent::Triggered, this, &ACSMenuPlayerController::SelectLeft);
	EnhancedInputComponent->BindAction(InputActionRight, ETriggerEvent::Triggered, this, &ACSMenuPlayerController::SelectRight);
	EnhancedInputComponent->BindAction(InputActionSelect, ETriggerEvent::Triggered, this, &ACSMenuPlayerController::Select);
	EnhancedInputComponent->BindAction(InputActionRestart, ETriggerEvent::Triggered, this, &ACSMenuPlayerController::Restart);
}

void ACSMenuPlayerController::SelectLeft()
{
	if (bHasSelected) return;
	if (bIsLeft) return;
	if (!PlayerSelectionRikimaru) return;
	if (!PlayerSelectionAyame) return;

	OnLeftSelected();
	PlayerSelectionRikimaru->SetSelected(true);
	PlayerSelectionAyame->SetSelected(false);
	bIsLeft = true;
}

void ACSMenuPlayerController::SelectRight()
{
	if (bHasSelected) return;
	if (!bIsLeft) return;
	if (!PlayerSelectionRikimaru) return;
	if (!PlayerSelectionAyame) return;

	OnRightSelected();
	PlayerSelectionRikimaru->SetSelected(false);
	PlayerSelectionAyame->SetSelected(true);
	bIsLeft = false;
}

void ACSMenuPlayerController::Select()
{
	if (bHasSelected) return;
	if (!PlayerSelectionRikimaru) return;
	if (!PlayerSelectionAyame) return;

	if (bIsLeft)
	{
		OnSelectRikimaruConfirmed();
		PlayerSelectionRikimaru->ConfirmSelect();
	}

	bHasSelected = true;
}

void ACSMenuPlayerController::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
