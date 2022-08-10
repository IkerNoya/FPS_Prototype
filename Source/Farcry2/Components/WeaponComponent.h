// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Audio")
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimMontage* AimFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* UnequipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* ReloadEmptyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* InspectMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UWeaponComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(ACharacterBase* TargetCharacter);
	
	//Move to gameplay tags in the future
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InspectWeapon();
	
protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	FORCEINLINE ACharacterBase* GetCharacter() const { return Character; }
};
