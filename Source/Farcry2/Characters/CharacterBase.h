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

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking, Running, Sprinting, Jumping, Mantling, Vaulting, Sliding
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemAction);

UCLASS()
class FARCRY2_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, Category = "Components|Custom")
	UInteractionComponent* InteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Custom")
	UInventoryComponent* Inventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState = EMovementState::Running;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RegularSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RegularCrouchSpeed = 250.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkingSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintingSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float ProneSpeed = 100.f;
	
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
	UFUNCTION(BlueprintCallable)
	void SwitchItem(int32 Slot);
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
	void StartSprinting();
	void StopSprinting();
	
	void StartWalking();
	void StopWalking();
	
	void StartCrouch();
	void EndCrouch();

	void StartJump();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();
	UFUNCTION(BlueprintImplementableEvent)
	void HandleSpeedChange(float NewSpeed);
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AItemBase* GetEquippedItem() const { return EquippedItem; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	UFUNCTION(BlueprintCallable)
	void SetMovementState(EMovementState State) { MovementState = State; }
	

private:
	UFUNCTION()
	void UpdateCurrentEquipment(int32 Slot);

};


