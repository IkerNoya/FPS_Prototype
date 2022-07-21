// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Items/ItemData.h"
#include "Item.generated.h"

UCLASS()
class FARCRY2_API AItemBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UItemData* ItemData;
public:
	virtual void HandleInteraction_Implementation(ACharacterBase* InteractionInstigator) override;
};