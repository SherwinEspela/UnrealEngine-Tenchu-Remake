// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RikimaruCharacter.h"
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
#include "Animation/PlayerAnimInstance.h"
#include "Utility/ActionCam.h"

ARikimaruCharacter::ARikimaruCharacter()
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

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	//GetCharacterMovement()->GroundFriction = 0.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 85.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CanCrouch();

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 20.f;
}

void ARikimaruCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AttachSword();
	bTakeCoverBoxInterpCompleted = false;
	Interactable = nullptr;
	bIsSwordEquipped = true;

	ActionCam = GetWorld()->SpawnActor<AActionCam>();
}

void ARikimaruCharacter::AttachSword()
{
	if (WeaponClass)
	{
		Katana = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		AttachSwordToSocket(FName("WEAPON_R"));
	}
}

void ARikimaruCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
	TakeCoverBoxInterp(DeltaTime);
}

void ARikimaruCharacter::TakeCoverBoxInterp(float DeltaTime)
{
	if (TenchuPlayerState != ETenchuPlayerStates::EPS_TakingCover) return;

	const FVector PlayerBoxLocation = TakeCoverBox->PlayerLocation->GetComponentLocation();
	const FVector CurrentPlayerLocation = GetActorLocation();
	const FVector TargetLocation(PlayerBoxLocation.X, PlayerBoxLocation.Y, CurrentPlayerLocation.Z);

	float InterpLocationX = FMath::FInterpTo(CurrentPlayerLocation.X, TargetLocation.X, DeltaTime, TakeCoverInterpSpeed);
	float InterpLocationY = FMath::FInterpTo(CurrentPlayerLocation.Y, TargetLocation.Y, DeltaTime, TakeCoverInterpSpeed);
	SetActorLocation(FVector(InterpLocationX, InterpLocationY, CurrentPlayerLocation.Z));
}

void ARikimaruCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARikimaruCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) return;
	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z = StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
}

void ARikimaruCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) return;

	float StartBaseEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z = StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
}

void ARikimaruCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	if (Camera)
	{
		Camera->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void ARikimaruCharacter::PlayerJump()
{
	if (!bIsJumping)
	{
		bIsJumping = true;
		if (PlayerAnimInstance)
		{
			PlayerAnimInstance->Montage_Play(MontageJump);
			PlayerAnimInstance->Montage_JumpToSection("JumpLong", MontageJump);
		}
	}
}

void ARikimaruCharacter::ToggleCrouch()
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_StealthAttacking) return;

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void ARikimaruCharacter::Crouch(bool bClientSimulation)
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_StealthAttacking) return;

	if (!bIsCrouched)
	{
		Super::Crouch(bClientSimulation);
	}
}

void ARikimaruCharacter::UnCrouch(bool bClientSimulation)
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_StealthAttacking) return;

	if (bIsCrouched)
	{
		Super::UnCrouch(bClientSimulation);
	}

}

void ARikimaruCharacter::StealthAttack()
{
	if (!CanInteract()) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_TakingCover) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;

	EnemyToStealthAttack = Cast<ATenchuEnemyCharacter>(Interactable);
	if (EnemyToStealthAttack == nullptr) return;

	TenchuPlayerState = ETenchuPlayerStates::EPS_Interacting;


	int SectionIndex = 1;
	if (bIsStealthDebugEnabled)
	{
		SectionIndex = StealthSectionIndexToDebug;
	}
	else {
		if (bIsStealthRandomized)
		{
			SectionIndex = FMath::RandRange(1, 2);
		}
		else {
			SectionIndex = CurrentStealthIndex;
			CurrentStealthIndex++;
			if (CurrentStealthIndex > 2)
			{
				CurrentStealthIndex = 1;
			}
		}
	}

	FName SectionName = GameUtilities::GetStealthEventSectionName(SectionIndex, bIsSwordEquipped);

	SetActorLocation(EnemyToStealthAttack->GetPlayerStealthKillLocation(SectionName, bIsSwordEquipped));
	const FRotator EnemyRotation = EnemyToStealthAttack->GetPlayerStealthKillRotation();
	SetActorRotation(EnemyRotation);
	GetController()->SetControlRotation(EnemyRotation);

	if (ActionCam)
	{
		TArray<FVector> Vectors = { GetActorLocation(), EnemyToStealthAttack->GetActorLocation() };
		FVector Average = UKismetMathLibrary::GetVectorArrayAverage(Vectors);
		Average.Z = EnemyToStealthAttack->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		ActionCam->SetActorLocation(Average);
		ActionCam->SetRandomView();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetViewTarget(ActionCam);
	}

	PlayStealthAttackAnimation(SectionName, SectionIndex);
}

void ARikimaruCharacter::PlayStealthAttackAnimation(FName SectionName, int SectionIndex)
{
	if (PlayerAnimInstance)
	{
		if (EnemyToStealthAttack->GetIsStealthAttackFromBack())
		{
			if (bIsSwordEquipped)
			{
				PlayerAnimInstance->Montage_Play(MontageStealthAttacks);
				PlayerAnimInstance->Montage_JumpToSection(SectionName, MontageStealthAttacks);
			}
			else {
				PlayerAnimInstance->Montage_Play(MontageStealthKillBackNoSword);
				PlayerAnimInstance->Montage_JumpToSection(SectionName, MontageStealthKillBackNoSword);
			}

			EEnemyDeathPose DeathPose = GameUtilities::GetDeathPose(SectionIndex, bIsSwordEquipped);
			EnemyToStealthAttack->StealthDeathBack(SectionName, DeathPose, bIsSwordEquipped);
		}
		else {
			AttachSwordToSocket(FName("WEAPON_R"));
			bIsSwordEquipped = true;

			FString FrontString("Front");
			FrontString.Append(FString::FromInt(SectionIndex));
			FName SectionName(*FrontString);

			PlayerAnimInstance->Montage_Play(MontageStealthAttacksFront);
			PlayerAnimInstance->Montage_JumpToSection(SectionName, MontageStealthAttacksFront);

			EEnemyDeathPose DeathPose = GameUtilities::GetStealthDeathFrontPose(SectionIndex);
			EnemyToStealthAttack->StealthDeathFront(SectionName, DeathPose, true);
		}
	}
}

void ARikimaruCharacter::TakeCover()
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_TakingCover) return;

	bTakeCoverBoxInterpCompleted = false;
	TenchuPlayerState = ETenchuPlayerStates::EPS_TakingCover;

	if (PlayerAnimInstance && MontageTakeCover)
	{
		TakeCoverBox = Cast<ATakeCoverBox>(Interactable);
		Interactable->Interact();
		bIsStandingWhenTakingCover = TakeCoverBox->bIsStanding;
		bIsTakingCoverFacingLeft = TakeCoverBox->bIsFacingLeft;

		FRotator TakeCoverBoxRotation = TakeCoverBoxRotation = TakeCoverBox->PlayerLocation->GetComponentRotation();
		SetActorRotation(TakeCoverBoxRotation);
	}
}

void ARikimaruCharacter::Interact()
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

void ARikimaruCharacter::SwordInteract()
{
	if (PlayerAnimInstance && PlayerAnimInstance->GetMovementSpeed() > 0.f) return;
	TenchuPlayerState = ETenchuPlayerStates::EPS_Interacting;

	if (PlayerAnimInstance && MontageSwordInteraction)
	{
		FName SectionName = bIsSwordEquipped ? FName("Sheathe") : FName("Unsheathe");
		PlayerAnimInstance->Montage_Play(MontageSwordInteraction);
		PlayerAnimInstance->Montage_JumpToSection(SectionName, MontageSwordInteraction);
		bIsSwordEquipped = !bIsSwordEquipped;
	}
}

void ARikimaruCharacter::HandleSwordUnsheatingCompleted()
{
	AttachSwordToSocket(FName("WEAPON_R"));
}

void ARikimaruCharacter::HandleSwordSheatingCompleted()
{
	AttachSwordToSocket(FName("SWORDCASE"));
}

void ARikimaruCharacter::AttachSwordToSocket(FName SocketName)
{
	if (Katana)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		Katana->AttachToComponent(GetMesh(), TransformRules, SocketName);
	}
}
