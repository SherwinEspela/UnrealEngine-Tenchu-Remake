// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Wall.h"
#include "Components/BoxComponent.h"
#include "TenchuCharacter.h"

AWall::AWall()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(GetRootComponent());
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AWall::OnPlayerBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AWall::OnPlayerEndOverlap);
}

void AWall::BeginPlay()
{
	Super::BeginPlay();
}

void AWall::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerBeginOverlap......"));
	
	/*if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		TenchuPlayer->SetActorToInteract(this);
	}*/
}

void AWall::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerEndOverlap......"));

	//if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	//if (TenchuPlayer)
	//{
	//	TenchuPlayer->RemoveActorToInteract();
	//}
}
