// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Inventory/Items/ItemObject.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class FARCRY2_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector CalculateSpawnLocation(AActor* Actor, bool bGroundClamp);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnItemFromActor(AActor* Actor, UItemObject* Item, bool GroundClamp);
};
