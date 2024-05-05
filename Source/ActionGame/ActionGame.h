// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("ACTION_Game"), STATGROUP_ACTION, STATCAT_Advanced);

static void LogOnScreen(const UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Time = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	const UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	const FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, NetPrefix + Msg);
	}
}