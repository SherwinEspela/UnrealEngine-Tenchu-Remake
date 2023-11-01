// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Props/Props.h"
#include "Components/BoxComponent.h"

AProps::AProps()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(GetRootComponent());
}
