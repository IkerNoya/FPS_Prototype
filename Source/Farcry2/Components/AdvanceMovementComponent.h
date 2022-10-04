// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvanceMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	None, Walking, Running, Sprinting, Jumping, Mantling, Vaulting, Sliding
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARCRY2_API UAdvanceMovementComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite)
	class ACharacterBase* Character = nullptr;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float RegularSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float RegularCrouchSpeed = 250.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float WalkingSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float SprintingSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float ProneSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float MantleHeight = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float MantleSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float QuickMantleSpeed = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float VaultSpeed = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Settings")
	float SlideImpulseForce = 600.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState = EMovementState::Running;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState PrevMovementState = EMovementState::Running;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState NextMovementState = EMovementState::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Animation")
	UAnimMontage* MantleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> LandingShake;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> MantlingShake;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> QuickMantleShake;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> VaultShake;
	
	UPROPERTY(VisibleAnywhere)
	float MantleDistance = 0;
	UPROPERTY(VisibleAnywhere)
	FVector MantlePosition = FVector::Zero();
	UPROPERTY(VisibleAnywhere)
	FVector VaultPosition;

	bool bBlockAdvanceMovement = false;

	
private:
	float OriginalGroundFriction;
	float OriginalBrakingDecelerationWalking;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement = nullptr;
	
public:
	// Sets default values for this component's properties
	UAdvanceMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
	void StartSprinting();
	void StopSprinting();
	
	void StartWalking();
	void StopWalking();
	
	void StartCrouch();
	void EndCrouch();

	void StartJump();
	
	UFUNCTION(BlueprintCallable)
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> Shake);
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeInputs(UInputComponent* PlayerInputComponent, ACharacter* OwnerCharacter);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EMovementState GetPreviousMovementState() const { return PrevMovementState; }
	UFUNCTION(BlueprintCallable)
	void SetMovementState(EMovementState State);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsBlockingAdvanceMovement() const { return bBlockAdvanceMovement; }
	UFUNCTION(BlueprintCallable)
	void ShouldBlockAdvanceMovement(bool Value);
	UFUNCTION(BlueprintCallable)
	bool IsDoingAdvanceMovement() const;
	UFUNCTION(BlueprintCallable)
	bool AreHandsBlocked() const;

private:
	float GetForwardInput()const;
	//Mantling
	void MantleCheck();
	bool IsQuickMantle() const;
	void MantleStart();
	void MantleMovement();
	//Vaulting
	void VaultCheck();
	void StartVault();
	void VaultMovement();
	//Sliding
	void StartSlide();
	void SlideUpdate();

	void ResetMovementValues();
};
