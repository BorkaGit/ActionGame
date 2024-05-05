// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"

#include "Net/UnrealNetwork.h"

ASPowerUpBase::ASPowerUpBase()
{
	SetReplicates(true);
}

void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

FText ASPowerUpBase::GetInteractText_Implementation(APawn* InstigatorPawn) const
{
	return FText::GetEmpty();
}

void ASPowerUpBase::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	SetActorHiddenInGame(!bIsActive);
}

void ASPowerUpBase::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ASPowerUpBase::RespawnPickupLogic()
{
	SetPowerUpState(false);
	
	FTimerHandle ReactivateTimerHandle;
	GetWorldTimerManager().SetTimer(ReactivateTimerHandle,this, &ASPowerUpBase::ShowPowerUp, 10.0f, false);
}

void ASPowerUpBase::SetPowerUpState_Implementation(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASPowerUpBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUpBase, bIsActive);
}

