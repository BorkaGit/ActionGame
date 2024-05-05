// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;
class USActionComponent;

UCLASS()
class ACTIONGAME_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASAICharacter();

	FORCEINLINE USAttributeComponent*	GetAttributeComponent	()		const { return AttributeComp;		}
	FORCEINLINE float					GetLowHealthPercentage	()		const { return LowHealthPercentage; }
	
protected:
	
	UPROPERTY(Transient)
	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

	void SetTargetActor(AActor* NewTarget);
	AActor* GetTargetActor() const;
	
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;
	
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values")
	float LowHealthPercentage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values")
	FName TargetActorKey = "TargetActor";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values")
	TSubclassOf<USWorldUserWidget> SpottedWidgetClass;
};
