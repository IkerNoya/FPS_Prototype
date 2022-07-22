// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/ItemData.h"
#include "Inventory/Item.h"


UMaterialInterface* UItemData::GetIcon() const
{
	return bIsRotated ? IconRotated : Icon;
}
