// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TakeCoverComponent.h"
#include "Components/BoxComponent.h"

UTakeCoverComponent::UTakeCoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(GetAttachmentRoot());

	PlayerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Location"));
	PlayerLocation->SetupAttachment(BoxComponent);
}


void UTakeCoverComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UTakeCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

