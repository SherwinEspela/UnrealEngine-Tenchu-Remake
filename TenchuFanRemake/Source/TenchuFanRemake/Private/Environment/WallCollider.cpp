// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/WallCollider.h"
#include "Components/BoxComponent.h"

AWallCollider::AWallCollider()
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
}

void AWallCollider::BeginPlay()
{
	Super::BeginPlay();
	
}
