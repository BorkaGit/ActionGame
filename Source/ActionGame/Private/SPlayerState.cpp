// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::AddCredits(const int32 Delta)
{
	const int32 NewCredits = FMath::Clamp(Credits + Delta, 0, 100);
	if (NewCredits != Credits)
	{
		Credits = NewCredits;
		OnCreditsChanged.Broadcast(this, Credits, Delta);
	}
}
