// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

UMaterialInterface* UItemObject::GetIcon() const
{
	return bIsRotated ? IconRotated : Icon;
}