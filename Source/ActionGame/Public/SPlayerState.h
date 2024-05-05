// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32,
                                               Delta);

UCLASS()
class ACTIONGAME_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	FORCEINLINE int32 GetCredits()		const		{ return Credits; }

	UFUNCTION(BlueprintCallable, Category = "Credits")
	FORCEINLINE bool IsEnoughCredits(int32 Price) const { return Credits >= Price; }

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(const int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);
	
protected:
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = "OnRep_Credits", BlueprintReadOnly, Category = "Credits", meta = (ClampMin = "0"))
	int32 Credits = 0;

	UFUNCTION()
	void OnRep_Credits(int32 OldCredits );
};
