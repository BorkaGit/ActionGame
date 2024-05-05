// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	Health = HealthMax;

	SetIsReplicatedByDefault(true);
}

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

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(float NewRage, float Delta)
{
	OnRageChanged.Broadcast(this, NewRage, Delta);
}

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
	const float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	const float ActualDelta = NewHealth - OldHealth;
	
	if ( GetOwner()->HasAuthority() )
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);	
		}

		if ( ActualDelta < 0.0f && !IsAlive() )
		{
			if ( ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>() )
			{
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return true;
}

void USAttributeComponent::ApplyRageChange(float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	const float ActualDelta = Rage - OldRage;
	MulticastRageChanged(Rage, ActualDelta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
	DOREPLIFETIME(USAttributeComponent, RageCoef);
}

