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
#include "Environment/TakeCoverBox.h"

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
	AnimInstance = GetMesh()->GetAnimInstance();
	AttachSword();
	bTakeCoverBoxInterpCompleted = false;
	Interactable = nullptr;
	bIsSwordEquipped = true;
}

void ATenchuCharacter::AttachSword()
{
	if (WeaponClass)
	{
		Katana = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		AttachSwordToSocket(FName("WEAPON_R"));
	}
}

void ATenchuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
	WalkSpeed = UKismetMathLibrary::VSizeXY(GetMovementComponent()->Velocity);
	TakeCoverBoxInterp(DeltaTime);
}

void ATenchuCharacter::TakeCoverBoxInterp(float DeltaTime)
{
	if (TenchuPlayerState != ETenchuPlayerStates::EPS_TakingCover) return;

	const FVector PlayerBoxLocation = TakeCoverBox->PlayerLocation->GetComponentLocation();
	const FVector CurrentPlayerLocation = GetActorLocation();
	const FVector TargetLocation(PlayerBoxLocation.X, PlayerBoxLocation.Y, CurrentPlayerLocation.Z);
	
	float InterpLocationX = FMath::FInterpTo(CurrentPlayerLocation.X, TargetLocation.X, DeltaTime, TakeCoverInterpSpeed);
	float InterpLocationY = FMath::FInterpTo(CurrentPlayerLocation.Y, TargetLocation.Y, DeltaTime, TakeCoverInterpSpeed);
	SetActorLocation(FVector(InterpLocationX, InterpLocationY, CurrentPlayerLocation.Z));
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

void ATenchuCharacter::PlayerJump()
{
	if (!bIsJumping)
	{
		bIsJumping = true;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageJump);
			AnimInstance->Montage_JumpToSection("JumpLong", MontageJump);
		}
	}
}

void ATenchuCharacter::ToggleCrouch()
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_StealthAttacking) return;
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
	if (!CanInteract()) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_TakingCover) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;

	EnemyToStealthAttack = Cast<ATenchuEnemyCharacter>(Interactable);
	if (EnemyToStealthAttack == nullptr) return;

	TenchuPlayerState = ETenchuPlayerStates::EPS_Interacting;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetViewTarget(EnemyToStealthAttack);

	SetActorLocation(EnemyToStealthAttack->GetPlayerStealthKillLocation(bIsSwordEquipped));
	const FRotator EnemyRotation = EnemyToStealthAttack->GetPlayerStealthKillRotation();
	SetActorRotation(EnemyRotation);
	GetController()->SetControlRotation(EnemyRotation);

	PlayStealthAttackAnimation();
}

void ATenchuCharacter::PlayStealthAttackAnimation()
{
	if (AnimInstance)
	{
		if (EnemyToStealthAttack->GetIsStealthAttackFromBack())
		{
			int SectionIndex = FMath::RandRange(1, 2);
			FName SectionName = GameUtilities::GetStealthEventSectionName(SectionIndex, bIsSwordEquipped);

			if (bIsSwordEquipped)
			{
				AnimInstance->Montage_Play(MontageStealthAttacks);
				AnimInstance->Montage_JumpToSection(SectionName, MontageStealthAttacks);
			}
			else {
				AnimInstance->Montage_Play(MontageStealthKillBackNoSword);
				AnimInstance->Montage_JumpToSection(SectionName, MontageStealthKillBackNoSword);
			}

			EEnemyDeathPose DeathPose = GameUtilities::GetDeathPose(SectionIndex, bIsSwordEquipped);
			EnemyToStealthAttack->StealthDeathBack(SectionName, DeathPose, bIsSwordEquipped);
		}
		else {
			AttachSwordToSocket(FName("WEAPON_R"));
			bIsSwordEquipped = true;

			int SectionIndex = bIsStealthDebugEnabled ? StealthSectionIndexToDebug : FMath::RandRange(1, 2);

			FString FrontString("Front");
			FrontString.Append(FString::FromInt(SectionIndex));
			FName SectionName(*FrontString);

			AnimInstance->Montage_Play(MontageStealthAttacksFront);
			AnimInstance->Montage_JumpToSection(SectionName, MontageStealthAttacksFront);

			EEnemyDeathPose DeathPose = GameUtilities::GetDeathPose(3, bIsSwordEquipped);
			EnemyToStealthAttack->StealthDeathFront(SectionName, DeathPose, true);
		}
	}
}

void ATenchuCharacter::TakeCover()
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_TakingCover) return;

	bTakeCoverBoxInterpCompleted = false;
	TenchuPlayerState = ETenchuPlayerStates::EPS_TakingCover;

	if (AnimInstance && MontageTakeCover)
	{
		TakeCoverBox = Cast<ATakeCoverBox>(Interactable);
		Interactable->Interact();
		bIsStandingWhenTakingCover = TakeCoverBox->bIsStanding;
		bIsTakingCoverFacingLeft = TakeCoverBox->bIsFacingLeft;

		FRotator TakeCoverBoxRotation = TakeCoverBoxRotation = TakeCoverBox->PlayerLocation->GetComponentRotation();
		SetActorRotation(TakeCoverBoxRotation);
	}
}

void ATenchuCharacter::Interact()
{
	if (!CanInteract()) return;

	switch (Interactable->GetInteractableType())
	{	
		case EInteractableType::EIT_Enemy:
			StealthAttack();
			break;

		case EInteractableType::EIT_TakeCoverBox:
			TakeCover();
			break;

		default:
			break;
	}
}

void ATenchuCharacter::SwordInteract()
{
	if (WalkSpeed > 0.f) return;
	TenchuPlayerState = ETenchuPlayerStates::EPS_Interacting;

	if (AnimInstance && MontageSwordInteraction)
	{
		FName SectionName = bIsSwordEquipped ? FName("Sheathe") : FName("Unsheathe");
		AnimInstance->Montage_Play(MontageSwordInteraction);
		AnimInstance->Montage_JumpToSection(SectionName, MontageSwordInteraction);
		bIsSwordEquipped = !bIsSwordEquipped;
	}
}

void ATenchuCharacter::HandleSwordUnsheatingCompleted()
{
	AttachSwordToSocket(FName("WEAPON_R"));
}

void ATenchuCharacter::HandleSwordSheatingCompleted()
{
	AttachSwordToSocket(FName("SWORDCASE"));
}

void ATenchuCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}
