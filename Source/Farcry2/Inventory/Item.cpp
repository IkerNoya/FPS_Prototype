// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Characters/CharacterBase.h"
#include "Components/InventoryComponent.h"

void AItemBase::HandleInteraction_Implementation(ACharacterBase* InteractionInstigator)
{
	IInteractionInterface::HandleInteraction_Implementation(InteractionInstigator);
	if(auto* Inventory = Cast<UInventoryComponent>(InteractionInstigator->FindComponentByClass(UInventoryComponent::StaticClass())))
	{

		if(!ItemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("HOLA PERRI2"))
			
		}
		if(Inventory->TryAddItem(ItemData))
		{
			Destroy();
		}
	}
}
