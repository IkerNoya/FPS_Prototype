// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable, Equippable
};
USTRUCT(BlueprintType)
struct FARCRY2_API FItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width =  1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;
};

UCLASS()
class FARCRY2_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItem Item;
};
