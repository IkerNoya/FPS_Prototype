// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemData.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FARCRY2_API UItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemInfo Item;
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
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Rotate();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FORCEINLINE bool IsRotated() const  { return bIsRotated; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FIntPoint GetDimensions();
};
