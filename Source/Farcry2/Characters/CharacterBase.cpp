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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	Camera->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionComponent)
	{
		InteractionComponent->SendInteractedItem.AddDynamic(this, &ACharacterBase::PickUpItem);
	}
	if (Inventory)
	{
		Inventory->OnEquipmentAdded.AddDynamic(this, &ACharacterBase::UpdateCurrentEquipment);
	}
	GetCharacterMovement()->MaxWalkSpeed = RegularSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = RegularCrouchSpeed;
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

void ACharacterBase::SetMovementState(EMovementState State)
{
	PrevMovementState = MovementState;
	switch (State) {
	case EMovementState::Walking:
		HandleSpeedChange(WalkingSpeed);
		break;
	case EMovementState::Running:
		HandleSpeedChange(RegularSpeed);
		break;
	case EMovementState::Sprinting:
		HandleSpeedChange(SprintingSpeed);
	 break;
	case EMovementState::Jumping:
		{
			switch (PrevMovementState) {
			case EMovementState::Walking:
				NextMovementState = EMovementState::Walking;
				break;
			case EMovementState::Running:
				NextMovementState = EMovementState::Running;
				break;
			case EMovementState::Sprinting:
				NextMovementState = EMovementState::Sprinting;
				break;
			default: ;
			}
		}
		break;
	default: ;
	}
	MovementState = State;
}

void ACharacterBase::UpdateCurrentEquipment(int32 Slot)
{
	SwitchItem(Slot);
}

void ACharacterBase::OnAttack()
{
	if (OnItemAction.IsBound())
	{
		OnItemAction.Broadcast();
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

void ACharacterBase::StartSprinting()
{
	if(GetCharacterMovement()->IsFalling())
	{
		NextMovementState = EMovementState::Sprinting;
		return;
	}
	if (bIsCrouched)
		EndCrouch();
	SetMovementState(EMovementState::Sprinting);
}

void ACharacterBase::StopSprinting()
{
	if (MovementState != EMovementState::Sprinting)
	{
		NextMovementState = EMovementState::Running;
		return;
	}
	SetMovementState(EMovementState::Running);
}

void ACharacterBase::StartWalking()
{
	if(GetCharacterMovement()->IsFalling())
	{
		NextMovementState = EMovementState::Walking;
		return;
	}
	SetMovementState(EMovementState::Walking);
}

void ACharacterBase::StopWalking()
{
	if (MovementState != EMovementState::Walking)
	{
		NextMovementState = EMovementState::Running;
		return;
	}
	SetMovementState(EMovementState::Running);
}

void ACharacterBase::StartCrouch()
{
	if(GetCharacterMovement()->IsFalling()) return;
	Crouch();
}

void ACharacterBase::EndCrouch()
{
	UnCrouch();
}

void ACharacterBase::StartJump()
{
	Jump();
	SetMovementState(EMovementState::Jumping);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetMovementState() == EMovementState::Jumping && !GetCharacterMovement()->IsFalling())
			SetMovementState(NextMovementState);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ACharacterBase::ToggleInventory);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacterBase::StartCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::StartSprinting);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &ACharacterBase::StartWalking);

	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACharacterBase::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::StopSprinting);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &ACharacterBase::StopWalking);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACharacterBase::OnAttack);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::Interact);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACharacterBase::LookUpAtRate);
}
