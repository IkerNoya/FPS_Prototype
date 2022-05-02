// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARCRY2_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	FVector PlayerLocation;
	FRotator PlayerRotation;
protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Distance = 50.f;
public:
	UInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void Interact();

protected:
	void BeginPlay() override;
	
private:
	void FindPlayerRotationAndLocation();
	FVector CalculateLineTraceEnd();

	UPROPERTY()
	class ACharacterBase* Character;
};
