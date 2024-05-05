// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupAction.h"

#include "SActionComponent.h"

void ASPowerupAction::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	const auto ActionComponent = USActionComponent::GetAction(InstigatorPawn);
	if ( ActionComponent && !ActionComponent->HasAction(ActionClass))
	{
		ActionComponent->AddAction(InstigatorPawn,ActionClass);
	}
}
