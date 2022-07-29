// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Items/ItemObject.h"
#include "Item.generated.h"

UCLASS()
class FARCRY2_API AItemBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ExposeOnSpawn))
	UItemObject* ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ExposeOnSpawn))
	TSubclassOf<UItemObject> ItemClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bIsEquipped = false;
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
public:
	AItemBase();
	UFUNCTION(BlueprintNativeEvent, Category = "Items")
	void UseItem();
	virtual void HandleInteraction_Implementation(ACharacterBase* InteractionInstigator) override;
	
	UFUNCTION(BlueprintCallable, Category = "Items")
	FORCEINLINE bool IsEquipped() const { return bIsEquipped; }
	UFUNCTION(BlueprintCallable, Category = "Items")
	FORCEINLINE void SetEquipStatus(bool Value) { bIsEquipped = Value; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UItemObject* GetItemData() const {return ItemData;} 
};

