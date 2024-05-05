// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerUpBase.generated.h"

UCLASS()
class ACTIONGAME_API ASPowerUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) const override;
	
public:	

	ASPowerUpBase();

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
	
	UFUNCTION()
	void ShowPowerUp();

	void RespawnPickupLogic();
	
	UFUNCTION(NetMulticast, Reliable)
	void SetPowerUpState(bool bNewIsActive);

};
