// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_Thorns.h"

#include "SAttributeComponent.h"

void UEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	const auto AttributeComponent = USAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.AddDynamic(this,&ThisClass::DamageAttacker);
}

void UEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	const auto AttributeComponent = USAttributeComponent::GetAttributes(Instigator);
	AttributeComponent->OnHealthChanged.RemoveDynamic(this,&ThisClass::DamageAttacker);
}

void UEffect_Thorns::DamageAttacker( AActor* Instigator, USAttributeComponent* OwningComponent, float NewHealth, float Delta)
{
	if (Instigator == OwningComponent->GetOwner())
	{
		return;
	}
	if (const auto AttributeComponent =  USAttributeComponent::GetAttributes(Instigator))
	{
		AttributeComponent->ApplyHealthChange(Instigator, FMath::RoundToFloat(Delta * DamageCoef));
	}
	
}
