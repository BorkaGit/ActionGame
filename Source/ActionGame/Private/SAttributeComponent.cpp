// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"


USAttributeComponent::USAttributeComponent()
{

	Health = 100.0f;
	HealthMax = 100.0f;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	UE_LOG(LogTemp, Warning, TEXT("Health :  %f + %f = %f"), Health - Delta, Delta, Health);
	
	return true;
}


