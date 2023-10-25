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

	PlayerSteathKillPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Player Stealth Kill Position"));
	PlayerSteathKillPosition->SetupAttachment(GetRootComponent());

	StealthKillCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Stealth Kill Camera Boom"));
	StealthKillCameraBoom->SetupAttachment(GetRootComponent());

	StealthKillCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Stealth Kill Camera"));
	StealthKillCamera->SetupAttachment(StealthKillCameraBoom);
}

void ATenchuEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyCloseWidget->SetVisibility(false);
}

void ATenchuEnemyCharacter::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATenchuCharacter* Player = Cast<ATenchuCharacter>(OtherActor);
	if (Player)
	{
		EnemyCloseWidget->SetVisibility(true);
		Player->SetEnemyToStealthAttack(this);
	}
}

void ATenchuEnemyCharacter::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATenchuCharacter* Player = Cast<ATenchuCharacter>(OtherActor);
	if (Player)
	{
		EnemyCloseWidget->SetVisibility(false);
		Player->RemoveEnemyToStealthAttack();
	}
}

void ATenchuEnemyCharacter::StealthDeath(FName SectionName, EEnemyDeathPose NewDeathPose)
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
		float RandomYaw = Stream.FRandRange(0.f, 360.f);
		StealthKillCameraBoom->SetWorldRotation(FRotator(0.f, RandomYaw, 0.f));
		StealthKillCameraBoom->SetWorldRotation(FRotator(Stream.FRandRange(15.f, -70.f), RandomYaw, 0.f));

		AnimInstance->Montage_Play(MontageStealthDeath);
		AnimInstance->Montage_JumpToSection(SectionName, MontageStealthDeath);

		DeathPose = NewDeathPose;
	}
}

FVector ATenchuEnemyCharacter::GetPlayerStealthKillLocation()
{
	return PlayerSteathKillPosition->GetComponentLocation();
}

FRotator ATenchuEnemyCharacter::GetPlayerStealthKillRotation()
{
	return PlayerSteathKillPosition->GetComponentRotation();
}
