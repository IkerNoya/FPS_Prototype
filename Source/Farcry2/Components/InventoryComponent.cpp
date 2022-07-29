// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Items.SetNum(Columns * Rows);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bIsDirty)
	{
		bIsDirty=false;
		OnInventoryChange.Broadcast();
	}
}

FSlot UInventoryComponent::IndexToTile(int32 Index)
{
	FSlot Slot;
	Slot.TileX = Index % Columns;
	Slot.TileY = Index / Columns;
	return Slot;
}

int32 UInventoryComponent::TileToIndex(FSlot Tile)
{
	return Tile.TileX + (Tile.TileY * Columns);
}

TMap<UItemObject*, FSlot> UInventoryComponent::GetAllItems()
{
	TMap<UItemObject*, FSlot> ItemsMap;
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsValid(Items[i]) && !ItemsMap.Contains(Items[i]))
		{
			ItemsMap.Add(Items[i], IndexToTile(i));
		}
	}
	return ItemsMap;
}

bool UInventoryComponent::IsRoomAvailable(UItemObject* Item, int32 TopLeftIndex)
{
	FSlot Slot = IndexToTile(TopLeftIndex);
	int32 LastXIndex = Slot.TileX + (Item->GetDimensions().X - 1);
	int32 LastYIndex = Slot.TileY + (Item->GetDimensions().Y - 1);
	for(int32 X = Slot.TileX; X <= LastXIndex; X++)
	{
		for(int32 Y = Slot.TileY; Y <= LastYIndex; Y++)
		{
			FSlot ResultingTile = FSlot(X, Y);
			if(!IsTileValid(ResultingTile)) return false;
			UItemObject* AuxItem = nullptr;
			bool IsIndexValid = GetItemAtIndex(TileToIndex(ResultingTile), AuxItem);
			if(!IsIndexValid) return false;
			if(IsValid(AuxItem)) return false;
		}
	}
	return true;
}

bool UInventoryComponent::TryAddItem(UItemObject* NewItem)
{
	if(!IsValid(NewItem)) return false;
	bool bWasRotated = false;
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsRoomAvailable(NewItem, i))
		{
			AddItemAt(NewItem, i);
			return true;
		}
	}
	NewItem->Rotate();
	for(int32 i = 0; i < Items.Num(); i++)
	{
		if(IsRoomAvailable(NewItem, i))
		{
			AddItemAt(NewItem, i);
			return true;
		}
	}
	return false;
}

void UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if(IsValid(Item))
	{
		for(int i = 0; i < Items.Num(); i++)
		{
			if(Items[i] == Item)
			{
				Items[i] = nullptr;
				bIsDirty=true;
			}
		}
	}
}

void UInventoryComponent::AddItemAt(UItemObject* NewItem, int32 TopLeftIndex)
{
	FSlot Slot = IndexToTile(TopLeftIndex);
	int32 LastXIndex = Slot.TileX + (NewItem->GetDimensions().X - 1);
	int32 LastYIndex = Slot.TileY + (NewItem->GetDimensions().Y - 1);
	for(int32 X = Slot.TileX; X <= LastXIndex; X++)
	{
		for(int32 Y = Slot.TileY; Y <= LastYIndex; Y++)
		{
			FSlot ResultingTile = FSlot(X, Y);
			Items[TileToIndex(ResultingTile)] = NewItem;
		}
	}
	bIsDirty = true;
}

bool UInventoryComponent::GetItemAtIndex(int32 Index, UItemObject*& ItemFound)
{
	ItemFound = nullptr;
	if(Items.IsValidIndex(Index))
	{
		ItemFound = Items[Index];
		return true;
	}
		return false;
}

bool UInventoryComponent::IsTileValid(FSlot Tile)
{
	return Tile.TileX >= 0 && Tile.TileY >= 0 && Tile.TileX < Columns && Tile.TileY  < Rows;
}

