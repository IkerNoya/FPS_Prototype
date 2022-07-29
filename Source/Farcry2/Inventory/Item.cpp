// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Characters/CharacterBase.h"
#include "Components/InventoryComponent.h"


void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	if (ItemClass)
	{
		ItemData = NewObject<UItemObject>(this, ItemClass);
	}
}

void AItemBase::BeginDestroy()
{
	Super::BeginDestroy();
}

AItemBase::AItemBase()
{
}

void AItemBase::UseItem_Implementation()
{
}

void AItemBase::HandleInteraction_Implementation(ACharacterBase* InteractionInstigator)
{
	IInteractionInterface::HandleInteraction_Implementation(InteractionInstigator);
}
