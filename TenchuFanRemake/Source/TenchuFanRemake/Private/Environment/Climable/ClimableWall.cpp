// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Climable/ClimableWall.h"
#include "Components/BoxComponent.h"
#include "Character/RikimaruCharacter.h"
#include "Engine/StaticMesh.h"

AClimableWall::AClimableWall()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	WarpTarget = CreateDefaultSubobject<USceneComponent>(TEXT("WarpTarget"));
	WarpTarget->SetupAttachment(GetRootComponent());
}

void AClimableWall::BeginPlay()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AClimableWall::OnPlayerBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AClimableWall::OnPlayerEndOverlap);
}

EInteractableType AClimableWall::GetInteractableType()
{
	return InteractableType;
}

void AClimableWall::SetWarpTargetTransform(FTransform Value)
{
	WarpTarget->SetRelativeTransform(Value);
}

void AClimableWall::SetWarpTargetPosition(FVector Value)
{
	WarpTarget->SetWorldLocation(Value);
}

void AClimableWall::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Rikimaru == nullptr) Rikimaru = Cast<ARikimaruCharacter>(OtherActor);
	if (Rikimaru)
	{
		Rikimaru->SetActorToInteract(this);
	}
}

void AClimableWall::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Rikimaru == nullptr) Rikimaru = Cast<ARikimaruCharacter>(OtherActor);
	if (Rikimaru)
	{
		Rikimaru->RemoveActorToInteract();
	}
}
