// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"


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
