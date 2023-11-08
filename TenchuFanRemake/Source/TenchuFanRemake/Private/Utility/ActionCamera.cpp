// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/ActionCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AActionCamera::AActionCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	RotationControl = CreateDefaultSubobject<USphereComponent>(TEXT("Rotation Control"));
	SetRootComponent(RotationControl);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RotationControl);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
}

void AActionCamera::BeginPlay()
{
	Super::BeginPlay();

	//FVector Original = CameraBoom->GetRelativeLocation();
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraBoom->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
}

void AActionCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActionCamera::SetRandomRotation()
{
	FRandomStream Stream(FMath::Rand());
	float RandomYaw = Stream.FRandRange(60.f, 300.f);
	SetActorRotation(FRotator(0.f, RandomYaw, 0.f));
	SetActorRotation(FRotator(Stream.FRandRange(15.f, -70.f), RandomYaw, 0.f));
}

