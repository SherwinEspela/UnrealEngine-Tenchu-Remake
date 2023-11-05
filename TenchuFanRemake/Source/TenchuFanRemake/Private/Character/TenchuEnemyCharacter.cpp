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
#include "AIController.h"

ATenchuEnemyCharacter::ATenchuEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

	bIsPatrolling = false;
}

void ATenchuEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsWaypointReached = true;
	bIsPatrolling = false;
	EnemyCloseWidget->SetVisibility(false);
	InteractableType = EInteractableType::EIT_Enemy;

	EnemyAnimInstance = GetMesh()->GetAnimInstance();

	EnemyAIController = Cast<AAIController>(GetController());

	if (NavigationWaypoints.Num() > 0)
	{
		bIsPatrolling = true;
		SelectNextWaypoint();
	}
}

void ATenchuEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ObserveIfWaypointReached();
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

void ATenchuEnemyCharacter::StealthDeath(EEnemyDeathPose NewDeathPose)
{
	if (EnemyAnimInstance)
	{
		OnEnemyDied.Broadcast();
		EnemyState = EEnemyStates::ES_Dead;
		bIsPatrolling = false;

		EnemyCloseWidget->SetVisibility(false);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetVisibility(false);
		
		FRandomStream Stream(FMath::Rand());
		float RandomYaw = Stream.FRandRange(60.f, 300.f);
		StealthKillCameraBoom->SetWorldRotation(FRotator(0.f, RandomYaw, 0.f));
		StealthKillCameraBoom->SetWorldRotation(FRotator(Stream.FRandRange(15.f, -70.f), RandomYaw, 0.f));

		DeathPose = NewDeathPose;
	}
}

void ATenchuEnemyCharacter::StealthDeathFront(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword)
{
	if (EnemyAnimInstance)
	{
		StealthDeath(NewDeathPose);
		EnemyAnimInstance->Montage_Play(MontageStealthDeathFront);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageStealthDeathFront);
	}
}

void ATenchuEnemyCharacter::StealthDeathBack(FName SectionName, EEnemyDeathPose NewDeathPose, bool bWithSword)
{
	if (EnemyAnimInstance)
	{
		StealthDeath(NewDeathPose);
		UAnimMontage* MontageToPlay = bWithSword ? MontageStealthDeath : MontageStealthDeathBackNoSword;
		EnemyAnimInstance->Montage_Play(MontageToPlay);
		EnemyAnimInstance->Montage_JumpToSection(SectionName, MontageToPlay);
	}
}

FVector ATenchuEnemyCharacter::GetPlayerStealthKillLocation(FName SectionName, bool bWithSword)
{
	PlayerSteathKillBackLocationWithSword.X = SectionName == FName("Behind1") ? -135.f : -180.f;

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
	//UKismetSystemLibrary::DrawDebugSphere(this, Player->GetActorLocation(), 10.f, 15.f, FColor::Red);

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

void ATenchuEnemyCharacter::ObserveIfWaypointReached()
{
	if (bIsWaypointReached) return;
	const double Distance = (CurrentWayPoint->GetActorLocation() - GetActorLocation()).Size();
	if (Distance <= PatrolAcceptanceRadius)
	{
		bIsWaypointReached = true;
		float RandomTime = FMath::RandRange(3.f, 7.f);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ATenchuEnemyCharacter::PatrolIdlingTimeFinished, RandomTime);
	}
}

void ATenchuEnemyCharacter::SelectNextWaypoint()
{
	if (!bIsPatrolling) return;

	if (EnemyAIController && !NavigationWaypoints.IsEmpty())
	{
		int RandomIndex = FMath::RandRange(0, NavigationWaypoints.Num() - 1);
		AActor* NextWayPoint = NavigationWaypoints[RandomIndex];

		if (NextWayPoint != CurrentWayPoint)
		{
			CurrentWayPoint = NextWayPoint;
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(CurrentWayPoint);
			MoveRequest.SetAcceptanceRadius(10.f);
			FNavPathSharedPtr NavPath;
			EnemyAIController->MoveTo(MoveRequest, &NavPath);
			bIsWaypointReached = false;
		}
		else {
			SelectNextWaypoint();
		}
	}
}

void ATenchuEnemyCharacter::PatrolIdlingTimeFinished()
{
	if (!bIsPatrolling) return;
	if (EnemyAnimInstance && MontageIdleTurn)
	{
		EnemyAnimInstance->Montage_Play(MontageIdleTurn);
	}
}

void ATenchuEnemyCharacter::HandleIdleTurningComplete()
{
	bIsPatrolling = true;
	SelectNextWaypoint();
}
