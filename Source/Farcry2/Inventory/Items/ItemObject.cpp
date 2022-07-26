// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

UMaterialInterface* UItemObject::GetIcon() const
{
	return bIsRotated ? IconRotated : Icon;
}

void UItemObject::Rotate()
{
	bIsRotated = !bIsRotated;
}

FIntPoint UItemObject::GetDimensions()
{
	if(!bIsRotated) return Item.Dimensions;
	return FIntPoint(Item.Dimensions.Y, Item.Dimensions.X);
}
