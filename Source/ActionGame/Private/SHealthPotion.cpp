// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (AttributeComp && AttributeComp->GetHealth() != AttributeComp->GetHealthMax() && PlayerState && PlayerState->IsEnoughCredits(CreditsPrice) )
	{
		RespawnPickupLogic();
		AttributeComp->ApplyHealthChange(this, HealthAmount);
		PlayerState->AddCredits(-CreditsPrice);
	}
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn) const
{
	const USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);
	if ( AttributeComponent && AttributeComponent->IsFullHealth() )
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT( "HealthPotion_InteractMessage", "Cost {0} Credits.Restores health to maximum."), CreditsPrice);
}

#undef LOCTEXT_NAMESPACE