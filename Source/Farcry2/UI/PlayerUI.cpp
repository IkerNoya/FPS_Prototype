// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
	if(!Player) return;

	PlayerHealthComponent = Cast<UHealthComponent>(Player->GetComponentByClass(UHealthComponent::StaticClass()));
	InventoryComponent = Cast<UInventoryComponent>(Player->GetComponentByClass(UInventoryComponent::StaticClass()));
}
