// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;

private:
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
