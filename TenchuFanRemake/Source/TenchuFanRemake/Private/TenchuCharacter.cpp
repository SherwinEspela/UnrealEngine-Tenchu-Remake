// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/TenchuEnemyCharacter.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameUtilities.h"

ATenchuCharacter::ATenchuCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f, 400.f, 0.f);
	MovementComponent->MaxWalkSpeed = 850.f;
	MovementComponent->MinAnalogWalkSpeed = 50.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	CanCrouch();

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 20.f;
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

	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
	WalkSpeed = UKismetMathLibrary::VSizeXY(GetMovementComponent()->Velocity);
}

void ATenchuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATenchuCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) return;
	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z = StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
}

void ATenchuCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) return;

	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z = StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
}

void ATenchuCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	if (Camera)
	{
		Camera->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void ATenchuCharacter::Jump()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogTemp, Warning, TEXT("Jump invoked......."))
		Super::Jump();
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageJump);
			AnimInstance->Montage_JumpToSection("JumpLong", MontageJump);
		}
	}
}

void ATenchuCharacter::ToggleCrouch()
{
	if (WalkSpeed > 0) return;
	if (bIsCrouched)
	{
		UnCrouch();
	} else {
		Crouch();
	}
}

void ATenchuCharacter::StealthAttack()
{
	if (EnemyToStealthAttack)
	{
		TenchuPlayerState = ETenchuPlayerStates::EPS_StealthAttacking;

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetViewTarget(EnemyToStealthAttack);

		SetActorLocation(EnemyToStealthAttack->GetPlayerStealthKillLocation());
		const FRotator EnemyRotation = EnemyToStealthAttack->GetPlayerStealthKillRotation();
		SetActorRotation(EnemyRotation);
		GetController()->SetControlRotation(EnemyRotation);

		PlayStealthAttackAnimation();
	}
}

void ATenchuCharacter::PlayStealthAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		int SectionIndex = FMath::RandRange(1, 2);
		FName SectionName = GameUtilities::GetStealthEventSectionName(SectionIndex);
		AnimInstance->Montage_Play(MontageStealthAttacks);
		AnimInstance->Montage_JumpToSection(SectionName, MontageStealthAttacks);

		EEnemyDeathPose DeathPose = GameUtilities::GetDeathPose(SectionIndex);
		EnemyToStealthAttack->StealthDeath(SectionName, DeathPose);
	}
}
