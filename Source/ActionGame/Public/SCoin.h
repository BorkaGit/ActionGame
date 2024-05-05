// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SCoin.generated.h"


UCLASS()
class ACTIONGAME_API ASCoin : public ASPowerUpBase
{
	GENERATED_BODY()

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int32 CreditsAmount = 10;
};
