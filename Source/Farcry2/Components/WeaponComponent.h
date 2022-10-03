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
	
	UPROPERTY()
	USkeletalMeshComponent *WeaponMesh = nullptr;
	
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
	UAnimSequenceBase* WeaponFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimMontage* AimFireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* UnequipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* WeaponReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* ReloadEmptyMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* EmptyWeaponReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Animation")
	UAnimSequenceBase* InspectMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|VFX")
	TSubclassOf<UCameraShakeBase> WeaponShotShake;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay|Settings")
	int CurrentAmmo = 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay|Settings")
	int CurrentSpareAmmo=90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Settings")
	int MaxMagazineAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Settings")
	int MaxSpareAmmo = 90;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay|Settings")
	bool bIsReloading = false;

	
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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACharacterBase* GetCharacter() const { return Character; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsReloading() const { return bIsReloading; }
	UFUNCTION(BlueprintCallable)
	void EndReload();
};
