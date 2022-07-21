// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Materials/MaterialInterface.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable, Equippable
};
USTRUCT(BlueprintType)
struct FARCRY2_API FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions =  FIntPoint::ZeroValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;
};

UCLASS()
class FARCRY2_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemInfo Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* IconRotated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class AItemBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsRotated;
};
