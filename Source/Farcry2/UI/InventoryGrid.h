// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

USTRUCT(BlueprintType)
struct FGridLines
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D Start = FVector2D::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D End = FVector2D::Zero();
};

USTRUCT(BlueprintType)
struct FGridSize
{
	GENERATED_BODY()
	FGridSize()
	{
		SizeX = 0;
		SizeY = 0;
	}
	FGridSize(float X, float Y)
	{
		SizeX = X;
		SizeY = Y;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SizeY;
};

UCLASS()
class FARCRY2_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	float SlotSize;
	//Make sure that instead of getting an inventory reference, i get the Rows and Columns from the HUD instead
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FGridLines> Lines;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetGridSize();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGridSize CalculateGridSize();

private:
	void CreateLineSegments();
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeGrid(UInventoryComponent* InventoryComponent, float Size);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();
};
