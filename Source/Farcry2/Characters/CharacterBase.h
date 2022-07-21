// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
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
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float TurnRateGamepad;

public:
	ACharacterBase();
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnItemAction OnItemAction;
	
protected:
	virtual void BeginPlay() override;

	void OnAttack();
	void Interact();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	
};