// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Farcry2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/Item.h"
#include "Inventory/Items/ItemObject.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	TurnRateGamepad = 45.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	Camera->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->SetOwnerNoSee(true);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	AdvanceMovementComponent = CreateDefaultSubobject<UAdvanceMovementComponent>(TEXT("AdvanceMovementComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	OriginalCameraLocation = Camera->GetRelativeLocation();
	if (InteractionComponent)
	{
		InteractionComponent->SendInteractedItem.AddDynamic(this, &ACharacterBase::PickUpItem);
	}
	if (Inventory)
	{
		Inventory->OnEquipmentAdded.AddDynamic(this, &ACharacterBase::UpdateCurrentEquipment);
	}

}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InteractionComponent)
	{
		InteractionComponent->SendInteractedItem.RemoveDynamic(this, &ACharacterBase::PickUpItem);
	}
	if (Inventory)
	{
		Inventory->OnEquipmentAdded.RemoveDynamic(this, &ACharacterBase::UpdateCurrentEquipment);
	}
	Super::EndPlay(EndPlayReason);
}

void ACharacterBase::UpdateCurrentEquipment(int32 Slot)
{
	SwitchItem(Slot);
}

void ACharacterBase::OnAttack()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	if (OnItemAction.IsBound())
	{
		OnItemAction.Broadcast();
	}
}

void ACharacterBase::OnSecondaryAction()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	bIsAiming = true;
	AdvanceMovementComponent->SetMovementState(EMovementState::Walking);
	AdvanceMovementComponent->ShouldBlockAdvanceMovement(true);
	if (OnItemSecondaryAction.IsBound())
	{
		OnItemSecondaryAction.Broadcast();
	}
}

void ACharacterBase::OnEndSecondaryAction()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	bIsAiming = false;
	AdvanceMovementComponent->SetMovementState(EMovementState::Running);
	AdvanceMovementComponent->ShouldBlockAdvanceMovement(false);
	if (OnItemSecondaryAction.IsBound())
	{
		OnItemSecondaryAction.Broadcast();
	}
}

void ACharacterBase::OnWeaponReload()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	if (OnReload.IsBound())
	{
		OnReload.Broadcast();
	}
}

void ACharacterBase::OnItemInspect()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	if (OnInspection.IsBound())
	{
		OnInspection.Broadcast();
	}
}

void ACharacterBase::OnItemMelee()
{
	if(AdvanceMovementComponent->AreHandsBlocked()) return;
	if (OnMelee.IsBound())
	{
		OnMelee.Broadcast();
	}
}

void ACharacterBase::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}

void ACharacterBase::SwitchItem(int32 Slot)
{
	if (!Inventory) return;
	UItemObject* Item = Inventory->SwitchActiveEquipmentSlot(Slot);
	if (EquippedItem)
	{
		if (Item && Inventory->GetActiveSlot() == Slot)
		{
			EquippedItem->Destroy();
			AItemBase* Equipment = Cast<AItemBase>(GetWorld()->SpawnActor(Item->ItemClass));
			if (Equipment)
			{
				if (auto* WeaponComponent = Cast<UWeaponComponent>(
					Equipment->GetComponentByClass(UWeaponComponent::StaticClass())))
				{
					EquippedItem = Equipment;
					WeaponComponent->AttachWeapon(this);
				}
			}
		}
		else
		{
			if (!Inventory->GetEquipmentInSlot(Inventory->GetActiveSlot()))
			{
				EquippedItem->Destroy();
				EquippedItem = nullptr;
			}
		}
	}
	else
	{
		if (Item && Slot == Inventory->GetActiveSlot())
		{
			AItemBase* Equipment = Cast<AItemBase>(GetWorld()->SpawnActor(Item->ItemClass));
			if (Equipment)
			{
				if (auto* WeaponComponent = Cast<UWeaponComponent>(
					Equipment->GetComponentByClass(UWeaponComponent::StaticClass())))
				{
					EquippedItem = Equipment;
					WeaponComponent->AttachWeapon(this);
				}
			}
		}
	}
}

void ACharacterBase::PickUpItem(AItemBase* Item)
{
	if (Inventory)
	{
		if (Inventory->TryAddItem(Item->GetItemData()))
		{
			Item->Destroy();
		}
	}
}

void ACharacterBase::MoveForward(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACharacterBase::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LeanRight()
{
	if(AdvanceMovementComponent->IsDoingAdvanceMovement() || bIsLeaning) return;
	AdvanceMovementComponent->ShouldBlockAdvanceMovement(true);
	AdvanceMovementComponent->SetMovementState(EMovementState::Walking);
	bIsLeaning = true;
	// SpringArm->bUsePawnControlRotation=false;
	HandleLeaning(45.f);
}

void ACharacterBase::LeanLeft()
{
	if(AdvanceMovementComponent->IsDoingAdvanceMovement() || bIsLeaning) return;
	AdvanceMovementComponent->ShouldBlockAdvanceMovement(true);
	AdvanceMovementComponent->SetMovementState(EMovementState::Walking);
	bIsLeaning = true;
	// SpringArm->bUsePawnControlRotation=false;
	HandleLeaning(-45.f);
}

void ACharacterBase::StopLeaning()
{
	bIsLeaning=false;
	HandleLeaning(0);
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	if(AdvanceMovementComponent)
		AdvanceMovementComponent->InitializeInputs(PlayerInputComponent, this);
	
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ACharacterBase::ToggleInventory);
	
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACharacterBase::OnAttack);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ACharacterBase::OnSecondaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &ACharacterBase::OnEndSecondaryAction);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACharacterBase::OnWeaponReload);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &ACharacterBase::OnItemInspect);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ACharacterBase::OnItemMelee);
	
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::Interact);
	
	PlayerInputComponent->BindAction("LeanRight", IE_Pressed, this, &ACharacterBase::LeanRight);
	PlayerInputComponent->BindAction("LeanRight", IE_Released, this, &ACharacterBase::StopLeaning);
	
	PlayerInputComponent->BindAction("LeanLeft", IE_Pressed, this, &ACharacterBase::LeanLeft);
	PlayerInputComponent->BindAction("LeanLeft", IE_Released, this, &ACharacterBase::StopLeaning);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACharacterBase::LookUpAtRate);
}
