// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	RootComponent = Mesh;
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
}

void AWeaponBase::DisableCollision()
{
	if(Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponBase::UseItem_Implementation()
{
	Super::UseItem_Implementation();
}

