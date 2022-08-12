// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Components/WeaponComponent.h"
#include "Inventory/Item.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponPickup, ACharacterBase*, PickUpCharacter);

UCLASS()
class FARCRY2_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Weapon")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UWeaponComponent* WeaponComponent;
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetMesh() const {return Mesh;}
protected:
	virtual void UseItem_Implementation() override;
	


};
