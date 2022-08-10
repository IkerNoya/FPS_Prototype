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
	if(!Character || !Character->GetController()) return;

	if(ProjectileClass != nullptr)
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
		}
	}

	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
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
}

void UWeaponComponent::Reload()
{
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




