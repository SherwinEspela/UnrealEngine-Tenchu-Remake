// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TenchuEnemyAnimInstance.h"

void UTenchuEnemyAnimInstance::NativeInitializeAnimation()
{
	SetNewIdlePose();
}

void UTenchuEnemyAnimInstance::SetNewIdlePose()
{
	int RandomNumber = FMath::RandRange(0, 3);
	
	switch (RandomNumber)
	{
		case 0:
			IdleType = EEnemyIdleType::EIT_Normal;
			break;

		case 1:
			IdleType = EEnemyIdleType::EIT_LoosenUp1;
			break;

		case 2:
			IdleType = EEnemyIdleType::EIT_LoosenUp2;
			break;

		case 3:
			IdleType = EEnemyIdleType::EIT_LoosenUp3;
			break;

		default:
			break;
	}
}


