// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if ( FromActor )
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorALive(AActor* Actor)
{
	const USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	return AttributeComponent ? AttributeComponent->IsAlive() : false;
}

USAttributeComponent::USAttributeComponent()
{
	
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

#if WITH_EDITOR
void USAttributeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Health = FMath::Clamp(Health, 0.0f, HealthMax);
	Rage = FMath::Clamp(Rage, 0.0f, RageMax);
}
#endif

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if ( !GetOwner()->CanBeDamaged() )
	{
		return false;
	}

	if ( Delta < 0.0f )
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if ( ActualDelta < 0.0f && !IsAlive() )
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if ( GameMode )
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return true;
}

void USAttributeComponent::ApplyRageChange(float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	const float ActualDelta = Rage - OldRage;
	OnRageChanged.Broadcast(this, Rage, ActualDelta);
}


