// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TakeCoverBox.h"
#include "Components/BoxComponent.h"
#include "TenchuCharacter.h"
#include "Components/WidgetComponent.h"

ATakeCoverBox::ATakeCoverBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxComponent->SetupAttachment(GetRootComponent());

	PlayerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Location"));
	PlayerLocation->SetupAttachment(BoxComponent);

	IndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Indicator Widget"));
	IndicatorWidget->SetupAttachment(BoxComponent);
	IndicatorWidget->SetDrawSize(FVector2D(40.f, 40.f));
	IndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ATakeCoverBox::BeginPlay()
{
	Super::BeginPlay();
	IndicatorWidget->SetVisibility(false);
	InteractableType = EInteractableType::EIT_TakeCoverBox;

	if (bIsActive)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerEndOverlap);
	}
}

void ATakeCoverBox::Interact()
{
	IndicatorWidget->SetVisibility(false);
}

EInteractableType ATakeCoverBox::GetInteractableType()
{
	return InteractableType;
}

void ATakeCoverBox::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;
	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		IndicatorWidget->SetVisibility(true);
		TenchuPlayer->SetActorToInteract(this);
	}
}

void ATakeCoverBox::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsActive) return;
	if (TenchuPlayer == nullptr) TenchuPlayer = Cast<ATenchuCharacter>(OtherActor);
	if (TenchuPlayer)
	{
		IndicatorWidget->SetVisibility(false);
		TenchuPlayer->RemoveActorToInteract();
	}
}