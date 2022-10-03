// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

#include "Farcry2Projectile.h"
#include "Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MuzzleOffset = FVector(100.f, 0.f, 10.f);
}

void UWeaponComponent::AttachWeapon(ACharacterBase* TargetCharacter)
{
	Character = TargetCharacter;
	if(auto* Weapon = Cast<AWeaponBase>(GetOwner()))
	{
		Weapon->DisableComponentsSimulatePhysics();
		Weapon->DisableCollision();
	}
	if(Character)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("gun")));
		Character->OnItemAction.AddDynamic(this, &UWeaponComponent::Fire);
		Character->OnReload.AddDynamic(this, &UWeaponComponent::Reload);
		Character->OnInspection.AddDynamic(this, &UWeaponComponent::InspectWeapon);
		
		AWeaponBase* WeaponBase = Cast<AWeaponBase>(GetOwner());
		if(WeaponBase) WeaponMesh = WeaponBase->GetMesh();
		else{UE_LOG(LogTemp, Warning, TEXT("FIUUMBA"));}
		
		if(EquipMontage)
		{
			if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(EquipMontage, "DefaultSlot");
			}
		}
	}
}

void UWeaponComponent::Fire()
{
	if(!Character || !Character->GetController() || bIsReloading) return;

	if(ProjectileClass != nullptr && CurrentAmmo > 0)
	{
		UWorld* const World = GetWorld();
		if(World)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			World->SpawnActor<AFarcry2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParameters);
			CurrentAmmo--;
		}
	}

	if(FireSound)
	{
		if(CurrentAmmo > 0)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	if(CurrentAmmo <= 0)
	{
		//No ammo anim
		return;
	}
	if(Character->IsAiming())
	{
		if(AimFireAnimation)
		{
			if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(AimFireAnimation, 1.f);
				AnimInstance->Montage_SetNextSection("Loop", "Tail", AimFireAnimation);
			}
		}
	}
	else
	{
		if(FireAnimation)
		{
			if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
				AnimInstance->Montage_SetNextSection("Loop", "Tail", FireAnimation);
			}
		}
	}
	if(WeaponMesh)
	{
		WeaponMesh->PlayAnimation(WeaponFireAnimation, false);
	}
	if(WeaponShotShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), WeaponShotShake, Character->GetActorLocation(), 0.f, 100.f);
	}
}

void UWeaponComponent::Reload()
{
	if(bIsReloading || CurrentAmmo >= MaxMagazineAmmo || CurrentSpareAmmo <= 0) return;
	
	bIsReloading=true;
	
	if(CurrentAmmo<=0)
	{
		if(ReloadEmptyMontage)
		{
			if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadEmptyMontage, "DefaultSlot");
			}
			if(WeaponMesh)
			{
				WeaponMesh->PlayAnimation(EmptyWeaponReloadAnimation, false);
			}
		}
	}
	else
	{
		if(ReloadMontage)
		{
			if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadMontage, "DefaultSlot");
			}
			if(WeaponMesh)
			{
				WeaponMesh->PlayAnimation(WeaponReloadAnimation, false);
			}
		}
	}
}

void UWeaponComponent::InspectWeapon()
{
	if(auto* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
	{
		AnimInstance->PlaySlotAnimationAsDynamicMontage(InspectMontage, "DefaultSlot");
	}
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character)
	{
		// Unregister from the OnUseItem Event
		Character->OnItemAction.RemoveDynamic(this, &UWeaponComponent::Fire);
		Character->OnReload.RemoveDynamic(this, &UWeaponComponent::Reload);
		Character->OnInspection.RemoveDynamic(this, &UWeaponComponent::InspectWeapon);

	}
}

void UWeaponComponent::EndReload()
{
	bIsReloading = false;
	int AuxAmmo = MaxMagazineAmmo - CurrentAmmo;
	if(CurrentSpareAmmo>=MaxMagazineAmmo)
		CurrentAmmo = MaxMagazineAmmo;
	else
		CurrentAmmo = CurrentSpareAmmo;
	CurrentSpareAmmo -= AuxAmmo;
	if(CurrentSpareAmmo < 0) CurrentSpareAmmo = 0;
}




