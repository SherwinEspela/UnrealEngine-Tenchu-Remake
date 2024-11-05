// Copyright 2024 Sherwin Espela. All rights reserved.


#include "Animation/Menu/TenchuCSAnimInstance.h"

void UTenchuCSAnimInstance::HandleAttachKatanaToHandNotification()
{
	OnAttachKatanaToHandNotified.Broadcast();
}
