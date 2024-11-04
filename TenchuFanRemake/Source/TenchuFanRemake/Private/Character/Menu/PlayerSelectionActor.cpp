// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/Menu/PlayerSelectionActor.h"
#include "Animation/Menu/TenchuCSAnimInstance.h"

APlayerSelectionActor::APlayerSelectionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerSelectionMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Selection Mesh"));
	PlayerSelectionMesh->SetupAttachment(RootComponent);
}

void APlayerSelectionActor::BeginPlay()
{
	Super::BeginPlay();
	
	PSAnimInstance = Cast<UTenchuCSAnimInstance>(PlayerSelectionMesh->GetAnimInstance());
}

void APlayerSelectionActor::SetSelected(bool Value)
{
	if (PSAnimInstance)
	{
		PSAnimInstance->SetSelected(Value);
	}
}
