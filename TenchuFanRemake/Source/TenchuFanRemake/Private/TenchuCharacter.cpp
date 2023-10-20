// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuCharacter.h"

ATenchuCharacter::ATenchuCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATenchuCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATenchuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATenchuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

