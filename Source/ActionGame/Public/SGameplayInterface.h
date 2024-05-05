// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"


UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class ACTIONGAME_API ISGameplayInterface
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
