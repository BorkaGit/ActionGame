// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONGAME_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASAICharacter();

	FORCEINLINE USAttributeComponent*	GetAttributeComponent	()		const { return AttributeComp;		}
	FORCEINLINE float					GetLowHealthPercentage	()		const { return LowHealthPercentage; }
	
protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;
	
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values")
	float LowHealthPercentage = 20.0f;
};
