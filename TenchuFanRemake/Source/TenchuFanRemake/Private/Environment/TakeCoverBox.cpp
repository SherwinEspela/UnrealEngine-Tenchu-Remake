// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TakeCoverBox.h"
#include "Components/BoxComponent.h"
#include "TenchuCharacter.h"

ATakeCoverBox::ATakeCoverBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerEndOverlap);

	PlayerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Location"));
	PlayerLocation->SetupAttachment(GetRootComponent());
}

void ATakeCoverBox::BeginPlay()
{
	Super::BeginPlay();
}

void ATakeCoverBox::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerBeginOverlap......"));

	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		TenchuPlayer->SetActorToInteract(this);
	}
}

void ATakeCoverBox::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Wall OnPlayerEndOverlap......"));

	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		TenchuPlayer->RemoveActorToInteract();
	}
}