// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AdvanceMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Custom")
	UInteractionComponent* InteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | Custom")
	UInventoryComponent* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | Custom")
	UAdvanceMovementComponent* AdvanceMovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | Custom")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float TurnRateGamepad;

	FVector OriginalCameraLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bEnableCameraShakes = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	float LeaningSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Settings")
	float CameraOffsetY = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Settings")
	float CameraOffsetZ = 30.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera|Settings")
	bool bIsLeaning = false;

private:
	UPROPERTY(VisibleAnywhere)
	AItemBase* EquippedItem = nullptr;
	bool bIsAiming = false;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemAction OnItemAction;
	FOnItemAction OnItemSecondaryAction;
	FOnItemAction OnReload;
	FOnItemAction OnMelee;
	FOnItemAction OnInspection;
	
	ACharacterBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnAttack();
	void OnSecondaryAction();
	void OnEndSecondaryAction();
	void OnWeaponReload();
	void OnItemInspect();
	void OnItemMelee();
	void Interact();
	
	UFUNCTION()
	void PickUpItem(class AItemBase* Item);
	UFUNCTION(BlueprintCallable)
	void SwitchItem(int32 Slot);
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void LeanRight();
	void LeanLeft();
	void StopLeaning();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> Shake);
	UFUNCTION(BlueprintImplementableEvent)
	void HandleLeaning(float Angle);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleSpeedChange(float NewSpeed);

	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AItemBase* GetEquippedItem() const { return EquippedItem; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool AreCameraShakesActive() const { return bEnableCameraShakes; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsAiming() const {return bIsAiming;}
	

private:
	UFUNCTION()
	void UpdateCurrentEquipment(int32 Slot);

};


