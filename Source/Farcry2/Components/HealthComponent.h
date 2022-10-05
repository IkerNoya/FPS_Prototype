// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class FARCRY2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;
private:
	float CurrentHealth = 0.f;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath;
	
	UHealthComponent();

	void TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser);
	
	
protected:
	virtual void BeginPlay() override;
	void Die();
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth() const { return CurrentHealth; }
};
