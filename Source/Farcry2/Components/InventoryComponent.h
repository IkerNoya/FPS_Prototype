// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Items/ItemData.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemData* Item = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsUsed = false;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class FARCRY2_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UItemData*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Columns = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Rows = 8;
	
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
	FORCEINLINE int32  GetColumns() const {return Columns;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetRows() const {return Rows;}
};
