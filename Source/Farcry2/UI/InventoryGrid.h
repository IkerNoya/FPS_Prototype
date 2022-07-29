// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Items/ItemObject.h"
#include "Slate/SlateBrushAsset.h"
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
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FIntPoint DraggedItemTopLeftTile;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bDrawDropLocation = true;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetGridSize();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGridSize CalculateGridSize();

	UFUNCTION(BlueprintCallable)
	void MousePositionInTile(FVector2D Position, bool& Right, bool &Down);
	UFUNCTION(BlueprintCallable)
	UItemObject* GetPayload(UDragDropOperation* Operation) const;
	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailableForPayload(UItemObject* Payload) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint GetItemTopLeftTile(UDragDropOperation* Operation, bool Right, bool Down, FVector2D MousePosition) const; 
	UFUNCTION(BlueprintCallable)
	void DropItemInInventory(UDragDropOperation* Operation);
	
private:
	void CreateLineSegments();
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeGrid(UInventoryComponent* InventoryComponent, float Size);
	
	UFUNCTION(BlueprintCallable)
	void DrawItemBox(FPaintContext Context, USlateBrushAsset* Brush, FLinearColor InvalidPositionColor, FLinearColor ValidPositionColor) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();
};
