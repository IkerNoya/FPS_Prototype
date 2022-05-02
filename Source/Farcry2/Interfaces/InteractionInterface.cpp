// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"

#include "Characters/CharacterBase.h"


// Add default functionality here for any IInteractionInterface functions that are not pure virtual.
void IInteractionInterface::HandleInteraction(ACharacterBase* InteractionInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted by %s"), InteractionInstigator? *InteractionInstigator->GetName() : TEXT("null actor"));
}
