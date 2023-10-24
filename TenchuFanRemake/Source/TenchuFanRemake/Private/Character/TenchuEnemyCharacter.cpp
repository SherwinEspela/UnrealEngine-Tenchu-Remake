// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TenchuEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TenchuCharacter.h"
#include "Animation/AnimMontage.h"

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

void ATenchuEnemyCharacter::StealthDeath()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageStealthDeath);
		AnimInstance->Montage_JumpToSection(FName("Behind1"), MontageStealthDeath);
	}
}
