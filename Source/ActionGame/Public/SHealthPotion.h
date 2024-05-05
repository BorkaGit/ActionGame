// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SHealthPotion.generated.h"


UCLASS()
class ACTIONGAME_API ASHealthPotion : public ASPowerUpBase
{
	GENERATED_BODY()

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) const override;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float HealthAmount = 25.0f;
	
	UPROPERTY(EditAnywhere,  meta = (ClampMin = "0"))
	int32 CreditsPrice = 10;
};
