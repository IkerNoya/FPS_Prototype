// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemObject.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon, Equipment
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Icon")
	FORCEINLINE UMaterialInterface* GetOriginalIcon() const { return Icon; }
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Rotate();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FORCEINLINE bool IsRotated() const  { return bIsRotated; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	FIntPoint GetDimensions();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Type")
	FORCEINLINE EItemType GetType() const {return Item.Type;}
};
