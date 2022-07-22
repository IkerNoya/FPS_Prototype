// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryGrid.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGridLines
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D Start = FVector2D::Zero();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D End = FVector2D::Zero();
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

private:
	void CreateLineSegments();
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializeGrid(UInventoryComponent* InventoryComponent, float Size);
	UFUNCTION(BlueprintNativeEvent)
	void Refresh();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveInventoryChangeMessage();
};
