// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::AddCredits(const int32 Delta)
{
	const int32 NewCredits = FMath::Clamp(Credits + Delta, 0, 100);
	if (NewCredits != Credits)
	{
		Credits = NewCredits;
		OnCreditsChanged.Broadcast(this, Credits, Delta);
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if ( SaveObject )
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if ( SaveObject )
	{
		Credits = SaveObject->Credits;
	}
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
