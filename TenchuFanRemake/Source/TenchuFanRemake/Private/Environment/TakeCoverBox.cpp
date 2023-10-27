// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TakeCoverBox.h"
#include "Components/BoxComponent.h"
#include "TenchuCharacter.h"
#include "Components/WidgetComponent.h"

ATakeCoverBox::ATakeCoverBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxComponent->SetupAttachment(GetRootComponent());
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerEndOverlap);

	PlayerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Location"));
	PlayerLocation->SetupAttachment(GetRootComponent());

	IndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Indicator Widget"));
	IndicatorWidget->SetupAttachment(GetRootComponent());
	IndicatorWidget->SetDrawSize(FVector2D(40.f, 40.f));
	IndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ATakeCoverBox::BeginPlay()
{
	Super::BeginPlay();

	IndicatorWidget->SetVisibility(false);
}

void ATakeCoverBox::TurnOffIndicatorWidgetVisibility()
{
	IndicatorWidget->SetVisibility(false);
}

void ATakeCoverBox::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		IndicatorWidget->SetVisibility(true);
		TenchuPlayer->SetActorToInteract(this);
	}
}

void ATakeCoverBox::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		IndicatorWidget->SetVisibility(false);
		TenchuPlayer->RemoveActorToInteract();
	}
}