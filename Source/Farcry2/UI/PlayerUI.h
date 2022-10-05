// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CharacterBase.h"
#include "Components/HealthComponent.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class FARCRY2_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	UHealthComponent* PlayerHealthComponent;
	UPROPERTY(BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ACharacterBase* Player;

protected:
	virtual void NativeConstruct() override;
	
	
};
