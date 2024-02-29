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
	void SetJumpTypeByMovementOffsetYaw();

public:
	FORCEINLINE float GetMovementSpeed() const { return MovementSpeed; }
	FORCEINLINE void SetJumpStarted() { bJumpStarted = true; }
	FORCEINLINE void SetJumpType(EJumpType Type) { JumpType = Type; }
	FORCEINLINE EJumpType GetJumpType() const { return JumpType; }
	FORCEINLINE void SetClimbing() { bIsClimbing = true; }

protected:
	void TurnInPlace();

	UFUNCTION(BlueprintCallable)
	void ResetRootYawOffset();

protected:
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;
	
	UPROPERTY(BlueprintReadOnly)
	float MovementOffsetYaw;

	UPROPERTY(BlueprintReadWrite)
	bool bJumpStarted = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJumpFlipping = false;

	UPROPERTY(BlueprintReadWrite)
	EJumpType JumpType = EJumpType::EJT_Default;

	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbing = false;

private:
	TObjectPtr<ARikimaruCharacter> PlayerCharacter;
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

private:
	// Turn in Place variables
	float CharacterYaw;
	float CharacterYawLastFrame;

	float RotationCurve;
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
};
