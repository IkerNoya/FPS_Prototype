// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemAction);

UCLASS()
class FARCRY2_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components|Mesh")
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(EditDefaultsOnly, Category = "Components|Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Components|Custom")
	UInteractionComponent* InteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Custom")
	UInventoryComponent* Inventory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float TurnRateGamepad;
private:
	UPROPERTY(VisibleAnywhere)
	AItemBase* EquippedItem = nullptr;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemAction OnItemAction;
	
	ACharacterBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnAttack();
	void Interact();
	
	UFUNCTION()
	void PickUpItem(class AItemBase* Item);
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AItemBase* GetEquippedItem() const { return EquippedItem; }

};
