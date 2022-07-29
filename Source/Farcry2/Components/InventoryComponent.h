// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Items/ItemObject.h"
// #include "Inventory/Items/ItemData.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
	FSlot(int32 X, int32 Y)
	{
		TileX = X;
		TileY = Y;
	}
	FSlot()
	{
		TileX = 0;
		TileY = 0;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TileY;;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChange);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class FARCRY2_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsDirty = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UItemObject*> Items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<int32, UItemObject*> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<int32, UItemObject*> Pockets;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Columns = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Rows = 8;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChange OnInventoryChange;
	
public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FSlot IndexToTile(int32 Index);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 TileToIndex(FSlot Tile);
	UFUNCTION(BlueprintCallable)
	TMap<UItemObject*, FSlot> GetAllItems();
	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObject* Item, int32 TopLeftIndex);
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemObject* NewItem);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemObject* Item);
	UFUNCTION(BlueprintCallable)
	void AddItemAt(UItemObject* NewItem, int32 TopLeftIndex);
	UFUNCTION(BlueprintCallable)
	void SetWeaponInSlot(int32 Slot, UItemObject* Weapon);
	UFUNCTION(BlueprintCallable)
	void SetItemInPocket(int32 Slot, UItemObject* Item);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32  GetColumns() const {return Columns;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetRows() const {return Rows;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UItemObject* GetItemInWeaponSlot(int32 Slot) const { return WeaponSlots[Slot]; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UItemObject* GetItemInPockets(int32 Slot) const { return Pockets[Slot]; }

private:
	bool  GetItemAtIndex(int32 Index, UItemObject*& ItemFound);
	bool IsTileValid(FSlot Tile);
};
