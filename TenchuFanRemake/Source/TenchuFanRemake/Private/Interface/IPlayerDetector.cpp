// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/IPlayerDetector.h"

// Add default functionality here for any IIPlayerDetector functions that are not pure virtual.

void IIPlayerDetector::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void IIPlayerDetector::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
