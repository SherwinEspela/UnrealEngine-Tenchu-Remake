// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Watermark.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class TENCHUFANREMAKE_API AWatermark : public AActor
{
	GENERATED_BODY()
	
public:	
	AWatermark();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WatermarkWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
