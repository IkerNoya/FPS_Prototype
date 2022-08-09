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
	if (auto* Owner = Cast<ACharacterBase>(GetOwner()))
	{
		Character = Owner;
		CharacterMovement = Character->GetCharacterMovement();
	}
	if (!Character || !CharacterMovement) return;
	CharacterMovement->MaxWalkSpeed = RegularSpeed;
	CharacterMovement->MaxWalkSpeedCrouched = RegularCrouchSpeed;
	OriginalGroundFriction = CharacterMovement->GroundFriction;
	OriginalBrakingDecelerationWalking = CharacterMovement->BrakingDecelerationWalking;
	// ...
}


// Called every frame
void UAdvanceMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Character)return;
	bool bFalling = CharacterMovement->IsFalling();
	UE_LOG(LogTemp, Warning, TEXT("Is Falling = %s"), bFalling ? TEXT("True") : TEXT("False"));
	switch (GetMovementState())
	{
	case EMovementState::Sprinting:
		if (GetForwardInput() <= 0)
		{
			StopSprinting();
		}
		break;
	case EMovementState::Jumping:
		if (!CharacterMovement->IsFalling())
		{
			PlayCameraShake(LandingShake);
			SetMovementState(NextMovementState);
		}
		break;
	case EMovementState::Mantling:
		MantleMovement();
		break;
	case EMovementState::Vaulting:
		VaultMovement();
		break;
	case EMovementState::Sliding:
		SlideUpdate();
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
	switch (State)
	{
	case EMovementState::Walking:
		ResetMovementValues();
		Character->HandleSpeedChange(WalkingSpeed);
		break;
	case EMovementState::Running:
		ResetMovementValues();
		Character->HandleSpeedChange(RegularSpeed);
		break;
	case EMovementState::Sprinting:
		ResetMovementValues();
		Character->HandleSpeedChange(SprintingSpeed);
		break;
	default: ;
	}
	if(State == EMovementState::Jumping || State == EMovementState::Mantling || State == EMovementState::Vaulting || State == EMovementState::Sliding)
	{
		switch (PrevMovementState)
		{
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
	MovementState = State;
}

float UAdvanceMovementComponent::GetForwardInput() const
{
	if (!Character)
		return 0.f;

	return FVector::DotProduct(Character->GetActorForwardVector(),
	                           CharacterMovement->GetLastInputVector());
}

void UAdvanceMovementComponent::MantleCheck()
{
	FRotator Rotation = FRotator::ZeroRotator;
	FVector EyesLocation;
	Character->GetController()->GetActorEyesViewPoint(EyesLocation, Rotation);
	EyesLocation = (EyesLocation + FVector(0, 0, 50.f)) + Character->GetActorForwardVector() * 100.f;
	FVector FeetLocation = Character->GetActorLocation() + FVector(
		0, 0, Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - MantleHeight);
	FeetLocation += Character->GetActorForwardVector() * 100.f;
	FQuat quat;
	FHitResult Hit;
	FCollisionShape Shape;
	Shape.MakeCapsule(20, 10);
	if (GetWorld()->SweepSingleByChannel(Hit, EyesLocation, FeetLocation, quat, ECC_Visibility, Shape))
	{
		MantleDistance = Hit.Distance;
		if (CharacterMovement->IsWalkable(Hit))
		{
			MantlePosition = Hit.ImpactPoint + FVector(
				0, 0, Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
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
	CharacterMovement->StopMovementImmediately();
	CharacterMovement->DisableMovement();
}

void UAdvanceMovementComponent::MantleMovement()
{
#pragma region CameraRotation
	// FRotator CurrentRotation = Character->GetController()->GetControlRotation();
	// FVector ActorLocation = FVector(Character->GetActorLocation().X, Character->GetActorLocation().Y, 0);
	// FVector MantleLocation = FVector(MantlePosition.X, MantlePosition.Y, 0);
	// FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, MantleLocation);	
	// Character->GetController()->SetControlRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 7.f));
#pragma endregion
	
	float Speed = IsQuickMantle() ? QuickMantleSpeed : MantleSpeed;
	Character->SetActorLocation(FMath::VInterpTo(Character->GetActorLocation(), MantlePosition,
	                                             GetWorld()->GetDeltaSeconds(), Speed));
	if (FVector::Distance(Character->GetActorLocation(), MantlePosition) < 8)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
		SetMovementState(NextMovementState);
	}
}

void UAdvanceMovementComponent::VaultCheck()
{
	FVector CharacterLocation = Character->GetActorLocation()/* + FVector(0, 0, 40.f)*/;
	FVector CharacterForward = Character->GetActorForwardVector() * 100.f;
	FHitResult Hit;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	if (GetWorld()->LineTraceSingleByObjectType(Hit, CharacterLocation, CharacterLocation + CharacterForward,
	                                            QueryParams))
	{
		FVector WallNormal;
		FVector WallLocation;
		WallLocation = Hit.Location;
		WallNormal = Hit.Normal;
		FRotator NormalRotator = UKismetMathLibrary::MakeRotFromX(WallNormal);
		FVector WallForward = NormalRotator.Vector() * -10.f;
		
		if (FHitResult SecondHit; GetWorld()->LineTraceSingleByObjectType(SecondHit, WallLocation + WallForward + FVector(0, 0, 200),
		                                                                  WallLocation + WallForward, QueryParams))
		{
			FVector NewWallForward = NormalRotator.Vector() * -50.f;
			if ((SecondHit.Location - WallLocation).Z > 60)
				return;
			
			if (FHitResult ThirdHit; GetWorld()->LineTraceSingleByObjectType(ThirdHit, WallLocation + NewWallForward + FVector(0, 0, 250),
			                                                                 WallLocation + NewWallForward - FVector(0, 0, 50), QueryParams))
				return;
			
		}
		VaultPosition = WallLocation + CharacterForward;
		StartVault();
	}
}

void UAdvanceMovementComponent::StartVault()
{
	SetMovementState(EMovementState::Vaulting);
	PlayCameraShake(VaultShake);
	CharacterMovement->StopMovementImmediately();
	CharacterMovement->DisableMovement();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UAdvanceMovementComponent::VaultMovement()
{
	Character->SetActorLocation(FMath::VInterpTo(Character->GetActorLocation(), VaultPosition,
	                                             GetWorld()->GetDeltaSeconds(), VaultSpeed));
	if (FVector::Distance(Character->GetActorLocation(), VaultPosition) < 8)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
		SetMovementState(NextMovementState);
		Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void UAdvanceMovementComponent::StartSlide()
{
	Character->Crouch();
	CharacterMovement->GroundFriction = 0.f;
	CharacterMovement->BrakingDecelerationWalking = 1400.f;
	CharacterMovement->MaxWalkSpeedCrouched = 0;
	FVector Velocity = CharacterMovement->Velocity;
	CharacterMovement->SetPlaneConstraintFromVectors(Velocity, Character->GetActorUpVector());
	CharacterMovement->SetPlaneConstraintEnabled(true);
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Character->GetActorLocation(), Character->GetActorLocation()+( Character->GetActorUpVector() * -200.f), ECC_Visibility);
	FVector Normal = FVector::CrossProduct(Hit.ImpactNormal, Character->GetActorRightVector());
	Normal *= -1.f;
	if(Normal.Z <= .02f)
	{
		CharacterMovement->AddImpulse(Normal * SlideImpulseForce, true);
	}
		SetMovementState(EMovementState::Sliding);
}

void UAdvanceMovementComponent::SlideUpdate()
{
	if(CharacterMovement->IsFalling())
	{
		SetMovementState(EMovementState::Jumping);
		return;
	}

	if(CharacterMovement->Velocity.Length() <= 35.f)
	{
		SetMovementState(NextMovementState);
	}
}

void UAdvanceMovementComponent::ResetMovementValues()
{
	CharacterMovement->MaxWalkSpeedCrouched = RegularCrouchSpeed;
	CharacterMovement->GroundFriction = OriginalGroundFriction;
	CharacterMovement->BrakingDecelerationWalking = OriginalBrakingDecelerationWalking;
	CharacterMovement->SetPlaneConstraintEnabled(false);
}

void UAdvanceMovementComponent::StartSprinting()
{
	if (GetForwardInput() <= 0) return;
	if (Character && Character->GetCharacterMovement()->IsFalling())
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
	if (Character && Character->GetCharacterMovement()->IsFalling())
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
	if (Character && Character->GetCharacterMovement()->IsFalling()) return;
	if (Character)
	{
		switch (GetMovementState()) { 
		case EMovementState::Walking:
			Character->Crouch();	
			break;
		case EMovementState::Running:
			Character->Crouch();
			break;
		case EMovementState::Sprinting:
			StartSlide();
			break;
		default: ;
		}
	}
}

void UAdvanceMovementComponent::EndCrouch()
{
	if (!Character)return;
	if(GetMovementState()!=EMovementState::Sliding)
		Character->UnCrouch();
	else
		NextMovementState = EMovementState::Walking;
}

void UAdvanceMovementComponent::StartJump()
{
	if (Character && GetMovementState() != EMovementState::Mantling && GetMovementState() != EMovementState::Vaulting)
	{
		if(GetMovementState()!=EMovementState::Jumping)
			VaultCheck();
		
		if (MovementState != EMovementState::Jumping && MovementState != EMovementState::Mantling && MovementState !=
			EMovementState::Vaulting)
		{
			if(Character->bIsCrouched)
			{
				Character->UnCrouch();
				SetMovementState(EMovementState::Running);
			}
			else
			{
				Character->Jump();
				PlayCameraShake(LandingShake);
				SetMovementState(EMovementState::Jumping);
			}
		}
		if(GetMovementState()!=EMovementState::Vaulting)
			MantleCheck();
	}
}

void UAdvanceMovementComponent::PlayCameraShake(TSubclassOf<UCameraShakeBase> Shake)
{
	if (Character && Shake && Character->AreCameraShakesActive())
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake, Character->GetActorLocation(), 0, 100);
	}
}
