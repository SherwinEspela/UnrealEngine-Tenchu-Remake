// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::Interact()
{
}

EInteractableType IInteractableInterface::GetInteractableType()
{
    return EInteractableType();
}
