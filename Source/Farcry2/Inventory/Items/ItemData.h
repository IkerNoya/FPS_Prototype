// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Materials/MaterialInterface.h"
#include "ItemData.generated.h"



UCLASS()
class FARCRY2_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	// FItemInfo Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class AItemBase> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsRotated;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UMaterialInterface* IconRotated;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Icon")
	UMaterialInterface* GetIcon() const;
};
