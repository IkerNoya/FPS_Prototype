// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceMovementComponent.h"

#include "Characters/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UAdvanceMovementComponent::UAdvanceMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAdvanceMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if(auto* Owner = Cast<ACharacterBase>(GetOwner()))
	{
		Character = Owner;
	}
	if(!Character) return;
	Character->GetCharacterMovement()->MaxWalkSpeed = RegularSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = RegularCrouchSpeed;
	// ...
	
}


// Called every frame
void UAdvanceMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!Character)return;
	switch (GetMovementState()) { 
	case EMovementState::Sprinting:
		if(GetForwardInput() <= 0)
		{
			StopSprinting();
		}
		break;
	case EMovementState::Jumping:
		if(!Character->GetCharacterMovement()->IsFalling())
		{
			PlayCameraShake(LandingShake);
			SetMovementState(NextMovementState);
		}
		break;
	case EMovementState::Mantling:
		MantleMovement();
		break;
	default: ;
	}
	// ...
}

void UAdvanceMovementComponent::InitializeInputs(UInputComponent* PlayerInputComponent, ACharacter* OwnerCharacter)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UAdvanceMovementComponent::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, Character, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &UAdvanceMovementComponent::StartCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &UAdvanceMovementComponent::StartSprinting);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &UAdvanceMovementComponent::StartWalking);

	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &UAdvanceMovementComponent::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &UAdvanceMovementComponent::StopSprinting);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &UAdvanceMovementComponent::StopWalking);
}

void UAdvanceMovementComponent::SetMovementState(EMovementState State)
{
	PrevMovementState = MovementState;
	switch (State) {
	case EMovementState::Walking:
		Character->HandleSpeedChange(WalkingSpeed);
		break;
	case EMovementState::Running:
		Character->HandleSpeedChange(RegularSpeed);
		break;
	case EMovementState::Sprinting:
		Character->HandleSpeedChange(SprintingSpeed);
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

float UAdvanceMovementComponent::GetForwardInput() const
{
	if(!Character)
		return 0.f;
		
	return FVector::DotProduct(Character->GetActorForwardVector(), Character->GetCharacterMovement()->GetLastInputVector());
}

void UAdvanceMovementComponent::MantleCheck()
{
	FRotator Rotation = FRotator::ZeroRotator;
	FVector EyesLocation;
	Character->GetController()->GetActorEyesViewPoint(EyesLocation,Rotation);
	EyesLocation =  (EyesLocation + FVector(0,0,50.f)) + Character->GetActorForwardVector() * 100.f;
	FVector FeetLocation = Character->GetActorLocation() + FVector(0,0, Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - MantleHeight);
	FeetLocation += Character->GetActorForwardVector() * 100.f;
	FQuat quat;
	FHitResult Hit;
	FCollisionShape Shape;
	Shape.MakeCapsule(20,10);
	if(GetWorld()->SweepSingleByChannel(Hit, EyesLocation, FeetLocation, quat, ECC_Visibility, Shape))
	{
		MantleDistance = Hit.Distance;
		if(Character->GetCharacterMovement()->IsWalkable(Hit))
		{
			MantlePosition = Hit.ImpactPoint + FVector(0,0,Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
			MantleStart();
		}
	}
}

bool UAdvanceMovementComponent::IsQuickMantle() const
{
	return MantleDistance > Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
}

void UAdvanceMovementComponent::MantleStart()
{
	SetMovementState(EMovementState::Mantling);
	PlayCameraShake(MantlingShake);
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->DisableMovement();
}

void UAdvanceMovementComponent::MantleMovement()
{
	// FRotator CurrentRotation = Character->GetController()->GetControlRotation();
	// FVector ActorLocation = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, 0);
	// FVector MantleLocation = FVector(MantlePosition.X, MantlePosition.Y, 0);
	// FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, MantleLocation);	
	// Character->GetController()->SetControlRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 7.f));
	float Speed = IsQuickMantle() ? QuickMantleSpeed : MantleSpeed;
	Character->SetActorLocation(FMath::VInterpTo(Character->GetActorLocation(), MantlePosition,GetWorld()->GetDeltaSeconds(), Speed));
	if(FVector::Distance(Character->GetActorLocation(), MantlePosition) < 8)
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		SetMovementState(NextMovementState);
	}
}

void UAdvanceMovementComponent::StartSprinting()
{
	if(GetForwardInput() <= 0) return;
	if(Character && Character->GetCharacterMovement()->IsFalling())
	{
		NextMovementState = EMovementState::Sprinting;
		return;
	}
	if (Character && Character->bIsCrouched)
		EndCrouch();
	
	SetMovementState(EMovementState::Sprinting);
}

void UAdvanceMovementComponent::StopSprinting()
{
	if (MovementState != EMovementState::Sprinting)
	{
		NextMovementState = EMovementState::Running;
		return;
	}
	SetMovementState(EMovementState::Running);
}
void UAdvanceMovementComponent::StartWalking()
{
	if(Character && Character->GetCharacterMovement()->IsFalling())
	{
		NextMovementState = EMovementState::Walking;
		return;
	}
	SetMovementState(EMovementState::Walking);
}

void UAdvanceMovementComponent::StopWalking()
{
	if (MovementState != EMovementState::Walking)
	{
		NextMovementState = EMovementState::Running;
		return;
	}
	SetMovementState(EMovementState::Running);
}
void UAdvanceMovementComponent::StartCrouch()
{
	if(Character && Character->GetCharacterMovement()->IsFalling()) return;
	if(Character) Character->Crouch();
}

void UAdvanceMovementComponent::EndCrouch()
{
	if(Character) Character->UnCrouch();
}
void UAdvanceMovementComponent::StartJump()
{
	if(Character && GetMovementState() != EMovementState::Mantling)
	{
		Character->Jump();
		if (MovementState != EMovementState::Jumping && MovementState != EMovementState::Mantling && MovementState != EMovementState::Vaulting)
			PlayCameraShake(LandingShake);
		SetMovementState(EMovementState::Jumping);

		MantleCheck();
	}
}

void UAdvanceMovementComponent::PlayCameraShake(TSubclassOf<UCameraShakeBase> Shake)
{
	if(Character && Character->AreCameraShakesActive())
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake, Character->GetActorLocation(), 0,100);
	}
}
