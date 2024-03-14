// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	if ( ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>() )
	{
		RespawnPickupLogic();
		PlayerState->AddCredits(CreditsAmount);
	}
	
}
