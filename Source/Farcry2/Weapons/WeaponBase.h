// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponPickup, ACharacterBase*, PickUpCharacter);

UCLASS()
class FARCRY2_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components|Collision")
	UBoxComponent* Box;
	UPROPERTY(EditDefaultsOnly, Category = "Components|Mesh")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Components|Custom")
	UWeaponComponent* WeaponComponent;

public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnWeaponPickup OnWeaponPickup;
protected:
	virtual void HandleInteraction_Implementation(ACharacterBase* InteractionInstigator) override;

};
