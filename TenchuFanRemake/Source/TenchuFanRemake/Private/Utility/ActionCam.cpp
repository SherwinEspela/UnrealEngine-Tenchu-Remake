// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActionCam.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

AActionCam::AActionCam()
{
	PrimaryActorTick.bCanEverTick = false;

	RotationControl = CreateDefaultSubobject<USphereComponent>(TEXT("Rotation Control"));
	SetRootComponent(RotationControl);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeLocation(FVector(DistanceFromCenter, 0.f, 0.f));
	CameraBoom->TargetArmLength = 80.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
}

void AActionCam::BeginPlay()
{
	Super::BeginPlay();
}

void AActionCam::SetRandomView()
{
	CameraBoom->SetRelativeLocation(FVector(FMath::FRandRange(-100.f, -150.f), 0.f, 0.f));

	FRandomStream Stream(FMath::Rand());
	float RandomYaw = Stream.FRandRange(60.f, 300.f);
	SetActorRotation(FRotator(0.f, RandomYaw, 0.f));
	SetActorRotation(FRotator(Stream.FRandRange(15.f, -70.f), RandomYaw, 0.f));
}
