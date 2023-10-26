// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/ColliderWall.h"
#include "Components/BoxComponent.h"

AColliderWall::AColliderWall()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(GetRootComponent());

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AColliderWall::OnPlayerBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AColliderWall::OnPlayerEndOverlap);
}

void AColliderWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AColliderWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColliderWall::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerBeginOverlap......"));
}

void AColliderWall::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerEndOverlap......"));
}
