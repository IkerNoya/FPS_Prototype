// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Items/ItemObject.h"
#include "Item.generated.h"

UCLASS()
class FARCRY2_API AItemBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ExposeOnSpawn))
	UItemObject* ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ExposeOnSpawn))
	TSubclassOf<UItemObject> ItemClass;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
public:
	AItemBase();
	
	virtual void HandleInteraction_Implementation(ACharacterBase* InteractionInstigator) override;
};

UCLASS()
class FARCRY2_API AStaticItem : public AItemBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
public:
	AStaticItem();
};
UCLASS()
class FARCRY2_API ASkeletalItem : public AItemBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* Mesh;
public:
	ASkeletalItem();
};
