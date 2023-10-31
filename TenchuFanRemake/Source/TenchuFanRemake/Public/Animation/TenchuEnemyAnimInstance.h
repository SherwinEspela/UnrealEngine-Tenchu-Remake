// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/TenchuAnimInstance.h"
#include "CustomEnums.h"
#include "TenchuEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TENCHUFANREMAKE_API UTenchuEnemyAnimInstance : public UTenchuAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void SetNewIdlePose();

protected:
	UPROPERTY(BlueprintReadOnly)
	EEnemyIdleType IdleType = EEnemyIdleType::EIT_Normal;
};
