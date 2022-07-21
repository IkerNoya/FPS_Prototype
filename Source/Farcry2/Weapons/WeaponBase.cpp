// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon Component"));
}

void AWeaponBase::HandleInteraction_Implementation(ACharacterBase* InteractionInstigator)
{
	IInteractionInterface::HandleInteraction_Implementation(InteractionInstigator);
	if(InteractionInstigator)
	{
		OnWeaponPickup.Broadcast(InteractionInstigator);
	}
}


