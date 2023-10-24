// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TenchuEnemyNinja.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

ATenchuEnemyNinja::ATenchuEnemyNinja()
{
	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	//SphereComponent->SetupAttachment(GetRootComponent());
	//SphereComponent->SetSphereRadius(150.f);

	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATenchuEnemyNinja::OnPlayerBeginOverlap);
	//SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATenchuEnemyNinja::OnPlayerEndOverlap);

	EnemyCloseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Enemy Close Widget"));
	EnemyCloseWidget->SetupAttachment(GetRootComponent());
}

void ATenchuEnemyNinja::BeginPlay()
{
	Super::BeginPlay();
}

void ATenchuEnemyNinja::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerBeginOverlap........%s"), *OtherActor->GetName());


}

void ATenchuEnemyNinja::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerEndOverlap........"));
}