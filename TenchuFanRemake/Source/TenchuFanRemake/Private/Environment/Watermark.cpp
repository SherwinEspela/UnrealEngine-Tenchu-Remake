// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Watermark.h"

// Sets default values
AWatermark::AWatermark()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWatermark::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWatermark::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWatermark::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

