// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RikimaruCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/TenchuEnemyCharacter.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "GameUtilities.h"
#include "Environment/TakeCoverBox.h"
#include "Environment/Climable/ClimableWall.h"
#include "Animation/PlayerAnimInstance.h"
#include "Utility/ActionCam.h"

ARikimaruCharacter::ARikimaruCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

	GetCharacterMovement()->GravityScale = 1.5f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->AirControlBoostMultiplier = 2.0f;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = 25.f;

	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchingSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = WalkingSpeed;
	//GetCharacterMovement()->GroundFriction = 0.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 85.f;

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
	
	GetCharacterMovement()->MaxWalkSpeedCrouched = RunningSpeed;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = RunningSpeed;
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

void ARikimaruCharacter::ClimbLedge()
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_HangingOnLedge) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Climbing) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Croucing) return;

	AClimableWall* Wall = Cast<AClimableWall>(Interactable);
	//if (Wall == nullptr) return;

	//bool IsCoincident = FVector::Coincident(GetActorForwardVector(), Wall->GetActorForwardVector(), 90.f);
	//if (IsCoincident)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Is COINCIDENT......"));
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("Is NOT COINCIDENT......"));
	//}

	/*FRotator Rotator1 = GetActorForwardVector().Rotation();
	FRotator Rotator2 = Wall->GetActorForwardVector().Rotation();
	UE_LOG(LogTemp, Warning, TEXT("Rotator1 Yaw = %f"), GetActorRotation().Yaw);*/

	FHitResult WallSurfaceHitResult;
	if (IsWallTraced(WallSurfaceHitResult))
	{
		

		/*FVector CurrentActorLocation = GetActorLocation();
		FVector CurrentForward = GetActorForwardVector();
		FRotator CurrentRotation = CurrentForward.Rotation();
		UE_LOG(LogTemp, Warning, TEXT("CurrentRotation.Yaw ======= %f"), CurrentRotation.Yaw);*/

		FVector TestPos = WallSurfaceHitResult.ImpactNormal - GetActorLocation() + WallSurfaceHitResult.ImpactPoint;
		TestPos = -TestPos;
		TestPos.Z = GetActorLocation().Z;
		DrawDebugSphere(GetWorld(), TestPos, 20.f, 20.f, FColor::Emerald, false, 5.f);

		//FRotator TestRotate = WallSurfaceHitResult.ImpactPoint.Rotation();
		//UE_LOG(LogTemp, Warning, TEXT("Yaw ==== %f"), TestRotate.Yaw);

		return;

		FHitResult WallTopHitResult;
		if (IsWallHeightTraced(WallTopHitResult))
		{
			FVector WallTopLocation = WallTopHitResult.ImpactPoint;
			FVector HeadSocketLocation = GetMesh()->GetSocketLocation(FName("HeadTopSocket"));
			bool IsWallHigherThanPlayer = WallTopLocation.Z > HeadSocketLocation.Z;
			if (IsWallHigherThanPlayer)
			{
				TenchuPlayerState = ETenchuPlayerStates::EPS_Climbing;
				
				if (Interactable)
				{

					if (Wall)
					{
						//FVector CurrentActorLocation = GetActorLocation();
						
						
						FVector WarpTargetPosition = WallSurfaceHitResult.ImpactPoint + WallSurfaceHitResult.ImpactNormal * ClimbStateWallSurfaceOffset;
						WarpTargetPosition.Z = WallTopLocation.Z - ClimbStateWallHeightOffset;
						Wall->SetWarpTargetPosition(WarpTargetPosition);
						ClimbTransformWarpTarget = Wall->GetWarpTargetTransform();
						OnClimbTransformWarpTargetAdded();
						GetCharacterMovement()->bUseControllerDesiredRotation = false;
						
						//DrawDebugSphere(GetWorld(), ClimbTransformWarpTarget.GetLocation(), 15.f, 10.f, FColor::Emerald, false, 5.f);
						

						if (PlayerAnimInstance && MontageClimbLedge) {
							PlayerAnimInstance->Montage_Play(MontageClimbLedge);
							GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
							GetCharacterMovement()->StopMovementImmediately();
							
						}
					}
				}
			}
		}
	}
}

void ARikimaruCharacter::ClimbLedgeTop()
{
	if (TenchuPlayerState != ETenchuPlayerStates::EPS_HangingOnLedge) return;

	AClimableWall* Wall = Cast<AClimableWall>(Interactable);
	if (Wall)
	{
		ClimbTransformWarpTarget = Wall->GetClimbLedgeTopWarpTargetTransform();
		OnClimbTransformWarpTargetAdded();
	}

	if (PlayerAnimInstance && MontageClimbLedge) {
		PlayerAnimInstance->Montage_Play(MontageClimbLedge);
		PlayerAnimInstance->Montage_JumpToSection(FName("ClimbTop1"));
	}
}

bool ARikimaruCharacter::IsWallTraced(FHitResult& OutHitResult)
{
	FVector StartLocation = GetActorLocation();// GetMesh()->GetSocketLocation(FName("PELVIS")); //
	//FVector EndLocation = GetMesh()->GetSocketLocation(FName("SocketWallTrace"));
	//DrawDebugSphere(GetWorld(), StartLocation, 10.f, 15.f, FColor::Blue, false, 5.f);
	//

	//FRotator BaseRotation = GetBaseAimRotation();
	//UE_LOG(LogTemp, Warning, TEXT("AimRotation ======= %f"), BaseRotation.Yaw);

	//FRotator DeltaRotation = BaseRotation;
	//DeltaRotation.Yaw = -BaseRotation.Yaw / 2.f;

	//UE_LOG(LogTemp, Warning, TEXT("DeltaRotation Yaw ======= %f"), DeltaRotation.Yaw);

	//FVector DeltaPos = DeltaRotation.Vector();

	//FVector EndLocation = StartLocation + (DeltaPos * 80.f); //(GetActorForwardVector() * 75.f);
	//EndLocation.Z = StartLocation.Z;
	
	//DrawDebugSphere(GetWorld(), EndLocation, 10.f, 15.f, FColor::Blue, false, 5.f);
	
	/*FVector ForwardVector = GetActorForwardVector();
	FRotator ForwardRotator = ForwardVector.Rotation();
	UE_LOG(LogTemp, Warning, TEXT("ForwardRotator.Yaw ======= %f"), ForwardRotator.Yaw);
	ForwardRotator.Yaw = 0.f;
	FVector ZeroRotateVector = ForwardRotator.Vector();*/
	FVector EndLocation = StartLocation + (GetActorForwardVector() * 150.f);
	return CanTraceWall(StartLocation, EndLocation, OutHitResult);
}

bool ARikimaruCharacter::IsWallHeightTraced(FHitResult& OutHitResult)
{
	FVector Offset = FVector{ 0.f, 0.f, 500.f };
	FVector StartLocation = GetActorLocation() + Offset + (GetActorForwardVector() * 75.f);
	FVector EndLocation = StartLocation - Offset;
	return CanTraceWall(StartLocation, EndLocation, OutHitResult);
}

bool ARikimaruCharacter::CanTraceWall(FVector StartLocation, FVector EndLocation, FHitResult &OutHit)
{
	TArray<AActor*> ActorsToIgnore;
	return UKismetSystemLibrary::SphereTraceSingle(
		this,
		StartLocation,
		EndLocation,
		10.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true
	);
}

//void ARikimaruCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

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

void ARikimaruCharacter::Move(FVector2D MovementVector, FVector ForwardDirection, FVector RightDirection)
{
	auto JumpType = PlayerAnimInstance->GetJumpType();
	if (GetCharacterMovement()->IsFalling() && JumpType == EJumpType::EJT_Default || !GetCharacterMovement()->IsFalling())
	{
		AddMovementInput(RightDirection, MovementVector.X);
	}
	AddMovementInput(ForwardDirection, MovementVector.Y);
	TenchuPlayerState = ETenchuPlayerStates::EPS_Moving;
}

void ARikimaruCharacter::LookAround(FVector2D LookAxisVector)
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Climbing) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_ClimbingOnLedgeTop) return;
	if (!GetCharacterMovement()->IsFalling())
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARikimaruCharacter::PlayerJump()
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Climbing) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_HangingOnLedge) return;

	if (CanInteract())
	{
		if (Interactable->GetInteractableType() == EInteractableType::EIT_ClimableWall)
		{
			ClimbLedge();
		}
	}
	else {
		PlayerAnimInstance->SetJumpStarted();
		PlayerAnimInstance->SetJumpType(EJumpType::EJT_Default);
		Jump();
	}
}

void ARikimaruCharacter::JumpFlip()
{
	if (GetCharacterMovement()->IsFalling()) return;
	if (PlayerAnimInstance) {
		PlayerAnimInstance->SetJumpTypeByMovementOffsetYaw();
	}
}

void ARikimaruCharacter::ToggleCrouch()
{
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
		GetCharacterMovement()->MaxWalkSpeed = CrouchingSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchingSpeed;
		GetCharacterMovement()->MinAnalogWalkSpeed = CrouchingSpeed;
	}
}

void ARikimaruCharacter::UnCrouch(bool bClientSimulation)
{
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_Interacting) return;
	if (TenchuPlayerState == ETenchuPlayerStates::EPS_StealthAttacking) return;

	if (bIsCrouched)
	{
		Super::UnCrouch(bClientSimulation);
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = RunningSpeed;
		GetCharacterMovement()->MinAnalogWalkSpeed = RunningSpeed;
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

	case EInteractableType::EIT_ClimableWall:
		ClimbLedgeTop();
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
