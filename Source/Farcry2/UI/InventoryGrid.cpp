// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryGrid.h"

#include "Components/InventoryComponent.h"

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

void UInventoryGrid::InitializeGrid(UInventoryComponent* InventoryComponent, float Size)
{
	Inventory = InventoryComponent;
	SlotSize = Size;
	SetGridSize();
	CreateLineSegments();
	Refresh();
	ReceiveInventoryChangeMessage();
}

void UInventoryGrid::Refresh_Implementation()
{
}
