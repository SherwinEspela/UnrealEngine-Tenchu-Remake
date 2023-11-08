// Fill out your copyright notice in the Description page of Project Settings.


#include "TenchuGameMode.h"
#include "EngineUtils.h"
#include "Character/TenchuEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TenchuCharacter.h"
#include "HUD/TenchuHUD.h"
#include "HUD/EnemyDetectorWidget.h"

void ATenchuGameMode::BeginPlay()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATenchuCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		Player = Cast<ATenchuCharacter>(FoundActors[0]);
	}

	TenchuHUD = Cast<ATenchuHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	GetAllEnemies();
}

void ATenchuGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FindClosestEnemy();
	if (ClosestEnemy && ClosestEnemy->EnemyState != EEnemyStates::ES_Dead)
	{
		if (EnemyDetectorWidget == nullptr) EnemyDetectorWidget = TenchuHUD->GetEnemyDetectorWidget();

		double DistanceFromEnemy = FVector::Distance(Player->GetActorLocation(), ClosestEnemy->GetActorLocation());
		float PercentDistanceFloat = DistanceFromEnemy / ClosestDistanceTotal * 100.f;
		
		int DeltaDistance = FMath::Floor(PercentDistanceFloat) - PercentDistanceOffset;
		EnemyDetectorWidget->DisplayIndicators(DeltaDistance < 100);

		int Range = 100 - DeltaDistance; 
		if (Range > 0 && Range <= 100)
		{
			EnemyDetectorWidget->UpdateDisplayFromRangeValue(Range);
		}
	}
}

void ATenchuGameMode::GetAllEnemies()
{	
	TArray<AActor*> FoundEnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATenchuEnemyCharacter::StaticClass(), FoundEnemyActors);

	if (FoundEnemyActors.Num() > 0)
	{
		for (AActor* EnemyActor : FoundEnemyActors)
		{
			ATenchuEnemyCharacter* Enemy = Cast<ATenchuEnemyCharacter>(EnemyActor);
			if (Enemy->EnemyState != EEnemyStates::ES_Dead)
			{
				Enemy->OnEnemyDied.AddDynamic(this, &ATenchuGameMode::HandleEnemyDied);
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

void ATenchuGameMode::HandleEnemyDied(ATenchuEnemyCharacter* Enemy)
{
	if (EnemyDetectorWidget == nullptr) EnemyDetectorWidget = TenchuHUD->GetEnemyDetectorWidget();
	if (EnemyDetectorWidget) EnemyDetectorWidget->DisplayIndicators(false);
	Enemy->OnEnemyDied.RemoveAll(this);
	GetAllEnemies();
}

void ATenchuGameMode::FindClosestEnemy()
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
			if(bIsDebugging) DrawDebugSphere(GetWorld(), ClosestEnemy->GetActorLocation(), 100.f, 15.f, FColor::Red, false, 0.2f);
			ClosestDistanceTotal = Distance;
		}
	}
}

void ATenchuGameMode::SetEnemyDetectorWidget(UEnemyDetectorWidget* Widget)
{
	EnemyDetectorWidget = Widget;
}
