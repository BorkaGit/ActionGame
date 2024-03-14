// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "Effect_Thorns.generated.h"


class USAttributeComponent;

UCLASS(HideCategories=(Effect))
class ACTIONGAME_API UEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void DamageAttacker( AActor* Instigator, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects", meta = ( ClampMin = "0.0", ClampMax = "1.0"))
	float DamageCoef = 0.0f;
};
