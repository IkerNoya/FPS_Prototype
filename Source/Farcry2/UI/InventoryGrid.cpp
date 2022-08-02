// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryGrid.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/InventoryComponent.h"
#include "Core/MainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void UInventoryGrid::CreateLineSegments()
{
	if(!Inventory) return;
	float X = 0;
	float Y = 0;
	
	for(int i = 0; i <=  Inventory->GetColumns(); i++)
	{
		X = i * SlotSize;
		FGridLines NewLine;
		NewLine.Start = FVector2D(X, 0);
		NewLine.End = FVector2D(X, Inventory->GetRows() * SlotSize);
		Lines.Add(NewLine);
	}
	
	for(int i = 0; i <=  Inventory->GetRows(); i++)
	{
		Y = i * SlotSize;
		FGridLines NewLine;
		NewLine.Start = FVector2D(0, Y);
		NewLine.End = FVector2D(Inventory->GetColumns() * SlotSize, Y);
		Lines.Add(NewLine);
	}
}

bool UInventoryGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	FVector2D MousePosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	bool bRight = false;
	bool bDown = false;
	MousePositionInTile(MousePosition, bRight, bDown);
	UItemObject* Item = Cast<UItemObject>(InOperation->Payload);

	DraggedItemTopLeftTile = GetItemTopLeftTile(InOperation, bRight, bDown, MousePosition);
	
	return true;
}

bool UInventoryGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UItemObject* Item = GetPayload(InOperation);
	if(IsRoomAvailableForPayload(Item))
	{
		Inventory->AddItemAt(Item, Inventory->TileToIndex(FSlot(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y)));
	}
	else
	{
		bool Success = Inventory->TryAddItem(Item);
		if(!Success)
		{
			if(auto* GameState = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				GameState->SpawnItemFromActor(Inventory->GetOwner(), Item, false);
			}
		}
	}
	//Return true to tell that operation was handled
	return true;
}

FGridSize UInventoryGrid::CalculateGridSize()
{
	if(!Inventory) return FGridSize();
	
	return FGridSize(SlotSize * Inventory->GetColumns(), SlotSize * Inventory->GetRows());
}

void UInventoryGrid::MousePositionInTile(FVector2D Position, bool& Right, bool& Down)
{
	Right = FMath::Fmod(static_cast<float>(Position.X), SlotSize) > SlotSize * .5f;
	Down = FMath::Fmod(static_cast<float>(Position.Y), SlotSize) > SlotSize * .5f;
}

UItemObject* UInventoryGrid::GetPayload(UDragDropOperation* Operation) const
{
	if(!Operation) return nullptr;
	return Cast<UItemObject>(Operation->Payload);
}

bool UInventoryGrid::IsRoomAvailableForPayload(UItemObject* Payload) const
{
	if(!Payload) return false;
	return Inventory->IsRoomAvailable(Payload, Inventory->TileToIndex(FSlot(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y)));
}

FIntPoint UInventoryGrid::GetItemTopLeftTile(UDragDropOperation* Operation, bool Right, bool Down, FVector2D MousePosition) const
{
	UItemObject* Item = GetPayload(Operation);
	if(!Item) return FIntPoint::ZeroValue;
	int32 RightValue = Right? 1 : 0;
	int32 DownValue = Down ? 1 : 0;
	int32 DimX = Item->GetDimensions().X - RightValue;
	int32 DimY = Item->GetDimensions().Y - DownValue;
	FIntPoint Dimensions = FIntPoint(FMath::Clamp(DimX, 0, DimX) / 2, FMath::Clamp(DimY, 0, DimY) / 2);
	FIntPoint Position = FIntPoint( FMath::TruncToInt(MousePosition.X / SlotSize), FMath::TruncToInt(MousePosition.Y / SlotSize));
	return Position - Dimensions;
}

void UInventoryGrid::DropItemInInventory(UDragDropOperation* Operation)
{
	UItemObject* Item = GetPayload(Operation);
	if(IsRoomAvailableForPayload(Item))
	{
		Inventory->AddItemAt(Item, Inventory->TileToIndex(FSlot(DraggedItemTopLeftTile.X, DraggedItemTopLeftTile.Y)));
	}
	else
	{
		bool Success = Inventory->TryAddItem(Item);
		if(!Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("HOLA"));
			if(auto* GameState = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				GameState->SpawnItemFromActor(Inventory->GetOwner(), Item, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Chau"));
		}
	}
}

void UInventoryGrid::DrawItemBox(FPaintContext Context, USlateBrushAsset* Brush, FLinearColor InvalidPositionColor, FLinearColor ValidPositionColor) const 
{
	if(!UWidgetBlueprintLibrary::IsDragDropping() || !bDrawDropLocation) return;
	UItemObject* Item = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());
	FLinearColor Color = IsRoomAvailableForPayload(Item) ? ValidPositionColor : InvalidPositionColor;
	FVector2D Position = FVector2D(DraggedItemTopLeftTile.X * SlotSize, DraggedItemTopLeftTile.Y * SlotSize);
	FVector2D Size = FVector2D(Item->GetDimensions().X * SlotSize, Item->GetDimensions().Y * SlotSize);
	UWidgetBlueprintLibrary::DrawBox(Context, Position, Size, Brush, Color);
}

void UInventoryGrid::InitializeGrid(UInventoryComponent* InventoryComponent, float Size)
{
	Inventory = InventoryComponent;
	SlotSize = Size;
	SetGridSize();
	CreateLineSegments();
	Refresh();
	if(Inventory)
	{
		Inventory->OnInventoryChange.AddDynamic(this, &UInventoryGrid::Refresh);
	}
}

