// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Props/DoubleSidedWall.h"

ADoubleSidedWall::ADoubleSidedWall()
{
	MirrorWallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror Wall Mesh"));
	MirrorWallMesh->SetupAttachment(GetRootComponent());
}
