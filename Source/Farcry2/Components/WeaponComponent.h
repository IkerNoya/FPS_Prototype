// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARCRY2_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY()
	class ACharacterBase* Character;
	
public:	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFarcry2Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Audio")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UWeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(ACharacterBase* TargetCharacter);
	
	//Move to gameplay tags in the future
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	FORCEINLINE ACharacterBase* GetCharacter() const { return Character; }
};
