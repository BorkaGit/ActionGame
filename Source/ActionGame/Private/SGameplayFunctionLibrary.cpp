// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributeComponent.h"

static TAutoConsoleVariable CVarImpulsePower(TEXT("su.ImpulsePower"), 300000.f, TEXT("Impulse power of the punch."), ECVF_Cheat);

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if ( USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor) )
	{
		AttributeComponent->ApplyRageChange(DamageAmount * AttributeComponent->GetRageCoef());
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if ( ApplyDamage(DamageCauser, TargetActor, DamageAmount) )
	{
		if (UPrimitiveComponent* HitComponent = HitResult.GetComponent(); HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName) )
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComponent->AddImpulseAtLocation(Direction * CVarImpulsePower.GetValueOnGameThread(), HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
