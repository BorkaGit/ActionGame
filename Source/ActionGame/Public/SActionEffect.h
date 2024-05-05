// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"


UCLASS()
class ACTIONGAME_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	USActionEffect();

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;
	
	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (ClampMin = "0.0"))
	float Duration = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect", meta = (ClampMin = "0.0"))
	float Period = 0.0f;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
	
private:
	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
	
};
