// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapons/Weapon.h"

ATenchuCharacter::ATenchuCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	CanCrouch();
}

void ATenchuCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttachSword();
}

void ATenchuCharacter::AttachSword()
{
	if (WeaponClass)
	{
		AWeapon* Katana = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (Katana)
		{
			FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
			Katana->AttachToComponent(GetMesh(), TransformRules, FName("WEAPON_R"));
		}
	}
}

void ATenchuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATenchuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATenchuCharacter::Jump()
{
	//if (bIsJumping == false)
	//{
	//	Jump();
	//	bIsJumping = true;
	//}
	Jump();
}

void ATenchuCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	} else {
		Crouch();
	}
}
