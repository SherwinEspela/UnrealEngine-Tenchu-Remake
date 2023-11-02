// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/HideBox.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

AHideBox::AHideBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxComponent->SetupAttachment(GetRootComponent());

	/*BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATakeCoverBox::OnPlayerEndOverlap);*/

	PlayerLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Location"));
	PlayerLocation->SetupAttachment(BoxComponent);

	IndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Indicator Widget"));
	IndicatorWidget->SetupAttachment(BoxComponent);
	IndicatorWidget->SetDrawSize(FVector2D(40.f, 40.f));
	IndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AHideBox::Interact()
{
}

EInteractableType AHideBox::GetInteractableType()
{
	return EInteractableType();
}

void AHideBox::BeginPlay()
{
	Super::BeginPlay();
	
}
