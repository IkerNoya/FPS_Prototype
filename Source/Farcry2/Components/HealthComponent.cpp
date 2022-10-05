// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "GameFramework/Character.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	if(CurrentHealth <= 0)
	{
		Die();
	}
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::Die()
{
	OnDeath.Broadcast();
}

