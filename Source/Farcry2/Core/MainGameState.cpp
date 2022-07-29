// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainGameState.h"

#include "Inventory/Item.h"

FVector AMainGameState::CalculateSpawnLocation(AActor* Actor, bool bGroundClamp)
{
	FVector SpawnLocation = Actor->GetActorLocation() + (Actor->GetActorForwardVector() * 150.f);
	FHitResult Hit;
	if(bGroundClamp && GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, SpawnLocation - FVector(0,0,1000), ECC_Visibility))
	{
		SpawnLocation = Hit.Location;
	}
	return SpawnLocation;
}
