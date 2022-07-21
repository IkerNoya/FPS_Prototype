// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Characters/CharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/InteractionInterface.h"

#define ECC_Interactable          ECC_GameTraceChannel3

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
}

void UInteractionComponent::Interact()
{
	if (!Character) return;

	FHitResult Hit;
	FindPlayerRotationAndLocation();
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FCollisionObjectQueryParams ObjectParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_Interactable));
	DrawDebugLine(GetWorld(), PlayerLocation, CalculateLineTraceEnd(), FColor::Red, false, 5);
	if (GetWorld()->LineTraceSingleByObjectType(Hit, PlayerLocation, CalculateLineTraceEnd(), ObjectParams,
	                                            TraceParams))
	{
		AActor* Actor = Hit.GetActor();
		if (Actor)
		{
			IInteractionInterface* InteractedObject = Cast<IInteractionInterface>(Actor);
			if (InteractedObject)
			{
				if (Actor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
				{
					InteractedObject->Execute_HandleInteraction(Actor, Character);
				}
			}
		}
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacterBase>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nharacter in component"));
	}
}

void UInteractionComponent::FindPlayerRotationAndLocation()
{
	if (Character)
	{
		if (auto* Controller = Cast<APlayerController>(Character->GetController()))
		{
			Controller->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
		}
	}
}

FVector UInteractionComponent::CalculateLineTraceEnd()
{
	return PlayerLocation + (PlayerRotation.Vector() * Distance);
}
