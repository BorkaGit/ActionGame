// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SPowerUpBase.h"
#include "SPowerupAction.generated.h"


class USAction;

UCLASS()
class ACTIONGAME_API ASPowerupAction : public ASPowerUpBase
{
	GENERATED_BODY()
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<USAction> ActionClass;
};
