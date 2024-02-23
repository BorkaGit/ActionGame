// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"


void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

ASPowerUpBase::ASPowerUpBase()
{
	

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

void ASPowerUpBase::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	SetActorHiddenInGame(!bNewIsActive);
}

