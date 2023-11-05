// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnemyDetectorComponent.h"
#include "EngineUtils.h"
#include "Character/TenchuEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TenchuCharacter.h"

UEnemyDetectorComponent::UEnemyDetectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyDetectorComponent::BeginPlay()
{
	Super::BeginPlay();	

	UE_LOG(LogTemp, Warning, TEXT("UEnemyDetectorComponent::BeginPlay..."));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATenchuCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		Player = Cast<ATenchuCharacter>(FoundActors[0]);
	}

	GetAllEnemies();
}

void UEnemyDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindClosestEnemy();
	if (ClosestEnemy && ClosestEnemy->EnemyState != EEnemyStates::ES_Dead)
	{
		double DistanceFromEnemy = FVector::Distance(Player->GetActorLocation(), ClosestEnemy->GetActorLocation());
		float PercentDistanceFloat = DistanceFromEnemy / ClosestDistanceTotal * 100.f;
		int DeltaDistance = FMath::Floor(PercentDistanceFloat) - PercentDistanceOffset;
		int Range = 100 - DeltaDistance;
		if (Range <= 100 && Range > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Distance ==== %i"), Range);
		}
	}
}

void UEnemyDetectorComponent::GetAllEnemies()
{
	TArray<AActor*> FoundEnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATenchuEnemyCharacter::StaticClass(), FoundEnemyActors);

	if (FoundEnemyActors.Num() > 0)
	{
		for (AActor* EnemyActor : FoundEnemyActors)
		{
			auto Enemy = Cast<ATenchuEnemyCharacter>(EnemyActor);
			if (Enemy->EnemyState != EEnemyStates::ES_Dead)
			{
				Enemy->OnEnemyDied.AddDynamic(this, &UEnemyDetectorComponent::GetAllEnemies);
				Enemies.Add(Enemy);
			}
		}

		if (Enemies.Num() > 0)
		{
			ClosestEnemy = Enemies[0];
			ClosestDistanceTotal = FVector::Distance(Player->GetActorLocation(), ClosestEnemy->GetActorLocation());
		}
	}
}

void UEnemyDetectorComponent::FindClosestEnemy()
{
	for (ATenchuEnemyCharacter* Enemy : Enemies)
	{
		double Distance = FVector::Distance(Player->GetActorLocation(), Enemy->GetActorLocation());
		double ClosestEnemyDistance = FVector::Distance(Player->GetActorLocation(), ClosestEnemy->GetActorLocation());
		if (Distance < ClosestEnemyDistance)
		{
			if (Enemy->EnemyState == EEnemyStates::ES_Dead) {
				continue;
			}

			ClosestEnemy = Enemy;
			DrawDebugSphere(GetWorld(), ClosestEnemy->GetActorLocation(), 100.f, 15.f, FColor::Red, false, 0.2f);
			ClosestDistanceTotal = Distance;
		}
	}
}
