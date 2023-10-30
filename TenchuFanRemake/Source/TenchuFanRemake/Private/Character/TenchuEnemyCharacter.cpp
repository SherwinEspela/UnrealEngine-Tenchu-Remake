// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TenchuEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "TenchuCharacter.h"
#include "Animation/AnimMontage.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameUtilities.h"
#include "Kismet/KismetSystemLibrary.h"

ATenchuEnemyCharacter::ATenchuEnemyCharacter()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(150.f);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATenchuEnemyCharacter::OnPlayerBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATenchuEnemyCharacter::OnPlayerEndOverlap);

	EnemyCloseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Enemy Close Widget"));
	EnemyCloseWidget->SetupAttachment(GetRootComponent());
	EnemyCloseWidget->SetVisibility(false);

	PlayerSteathKillPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Stealth Kill Position"));
	PlayerSteathKillPosition->SetupAttachment(GetRootComponent());

	PlayerSteathKillPositionFront = CreateDefaultSubobject<USceneComponent>(TEXT("Stealth Kill Position Front"));
	PlayerSteathKillPositionFront->SetupAttachment(GetRootComponent());

	PlayerSteathKillPositionBack = CreateDefaultSubobject<USceneComponent>(TEXT("Stealth Kill Position Behind"));
	PlayerSteathKillPositionBack->SetupAttachment(GetRootComponent());

	StealthKillCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Stealth Kill Camera Boom"));
	StealthKillCameraBoom->SetupAttachment(GetRootComponent());

	StealthKillCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Stealth Kill Camera"));
	StealthKillCamera->SetupAttachment(StealthKillCameraBoom);
}

void ATenchuEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyCloseWidget->SetVisibility(false);
	InteractableType = EInteractableType::EIT_Enemy;
}

void ATenchuEnemyCharacter::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATenchuCharacter* Player = Cast<ATenchuCharacter>(OtherActor);
	if (Player)
	{
		GetStealthPosition(OtherActor);
		EnemyCloseWidget->SetVisibility(true);
		Player->SetActorToInteract(this);
	}
}

void ATenchuEnemyCharacter::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATenchuCharacter* Player = Cast<ATenchuCharacter>(OtherActor);
	if (Player)
	{
		EnemyCloseWidget->SetVisibility(false);
		Player->RemoveActorToInteract();
	}
}

void ATenchuEnemyCharacter::StealthDeath(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		EnemyCloseWidget->SetVisibility(false);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetVisibility(false);
		
		FRandomStream Stream(FMath::Rand());
		float RandomYaw = Stream.FRandRange(60.f, 300.f);
		StealthKillCameraBoom->SetWorldRotation(FRotator(0.f, RandomYaw, 0.f));
		StealthKillCameraBoom->SetWorldRotation(FRotator(Stream.FRandRange(15.f, -70.f), RandomYaw, 0.f));

		UAnimMontage* MontageToPlay = bWithSword ? MontageStealthDeath : MontageStealthDeathBackNoSword;
		AnimInstance->Montage_Play(MontageToPlay);
		AnimInstance->Montage_JumpToSection(SectionName, MontageToPlay);

		DeathPose = NewDeathPose;
	}
}

FVector ATenchuEnemyCharacter::GetPlayerStealthKillLocation(bool bWithSword)
{
	PlayerSteathKillPositionBack->SetRelativeLocation(bWithSword ? PlayerSteathKillBackLocationWithSword : PlayerSteathKillBackLocationNoSword);
	FVector StealthLocation = bIsStealthAttackFromBack ? PlayerSteathKillPositionBack->GetComponentLocation() : PlayerSteathKillPositionFront->GetComponentLocation();
	return StealthLocation;
}

FRotator ATenchuEnemyCharacter::GetPlayerStealthKillRotation()
{
	FRotator StealthRotation = bIsStealthAttackFromBack ? PlayerSteathKillPositionBack->GetComponentRotation() : PlayerSteathKillPositionFront->GetComponentRotation();
	return StealthRotation;
}

void ATenchuEnemyCharacter::Interact()
{
}

EInteractableType ATenchuEnemyCharacter::GetInteractableType()
{
	return InteractableType;
}

void ATenchuEnemyCharacter::GetStealthPosition(AActor* Player)
{
	UKismetSystemLibrary::DrawDebugSphere(this, Player->GetActorLocation(), 10.f, 15.f, FColor::Red);

	const FVector Right = GetActorRightVector();
	const FVector ToHit = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// Right * ToHit = |Right||ToHit| * cos(theta)
	const double CosTheta = FVector::DotProduct(Right, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	
	const FVector CrossProduct = FVector::CrossProduct(Right, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	bIsStealthAttackFromBack = Theta > 0.f; // Positive values

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Red,
			FString::Printf(TEXT("Theta: %f"), Theta)
		);
	}

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Right * 100.f, 5.f, FColor::Blue, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 100.f, 5.f, FColor::Green, 5.f);*/
}
