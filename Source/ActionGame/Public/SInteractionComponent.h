// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;
	
public:
	
	USInteractionComponent();
	
	void PrimaryInteract();
	
	private:
	
	UPROPERTY(Transient)
	AActor* FocusedActor = nullptr;

	UPROPERTY(Transient)
	USWorldUserWidget* DefaultWidgetInstance = nullptr;
};
