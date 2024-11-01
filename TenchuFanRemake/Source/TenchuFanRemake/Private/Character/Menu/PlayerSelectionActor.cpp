// Copyright 2024 Sherwin Espela. All rights reserved.

#include "Character/Menu/PlayerSelectionActor.h"

APlayerSelectionActor::APlayerSelectionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerSelectionMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Selection Mesh"));
	PlayerSelectionMesh->SetupAttachment(RootComponent);
}

void APlayerSelectionActor::BeginPlay()
{
	Super::BeginPlay();
	
}
