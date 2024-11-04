// Copyright 2024 Sherwin Espela. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSelectionActor.generated.h"

class USkeletalMeshComponent;
class UTenchuCSAnimInstance;

UCLASS()
class TENCHUFANREMAKE_API APlayerSelectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerSelectionActor();

	void SetSelected(bool Value);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Selection Mesh")
	USkeletalMeshComponent* PlayerSelectionMesh;

	UTenchuCSAnimInstance* PSAnimInstance;
};
