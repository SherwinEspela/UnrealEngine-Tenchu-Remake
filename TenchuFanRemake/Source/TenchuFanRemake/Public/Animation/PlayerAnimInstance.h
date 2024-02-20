// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomEnums.h"
#include "PlayerAnimInstance.generated.h"

class ARikimaruCharacter;
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

public:
	FORCEINLINE float GetMovementSpeed() const { return MovementSpeed; }
	FORCEINLINE void SetJumpStarted() { bJumpStarted = true; }
	FORCEINLINE void SetJumpType(EJumpType Type) { JumpType = Type; }

protected:
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;
	
	UPROPERTY(BlueprintReadOnly)
	float MovementOffsetYaw;

	UPROPERTY(BlueprintReadWrite)
	bool bJumpStarted = false;

	UPROPERTY(BlueprintReadOnly)
	EJumpType JumpType = EJumpType::EJT_Default;

private:
	TObjectPtr<ARikimaruCharacter> PlayerCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
