// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionEffect.h"


ASMagicProjectile::ASMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 10.0f;
}


void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	AudioComponent->SetSound(FlightSound);
}

void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (const auto InstigatorCharacter = Cast<ASCharacter>(GetInstigator()))
	{
		UGameplayStatics::SpawnEmitterAttached(CastParticle,InstigatorCharacter->GetMesh(),FName("Muzzle_01"));
	}
	
	AudioComponent->Play();
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	APawn* InstigatorActor = GetInstigator();
	if (OtherActor != InstigatorActor)
	{
		AudioComponent->Stop();
		AudioComponent->SetSound(ImpactSound);
		AudioComponent->Play();

		USActionComponent* ActionComponent = USActionComponent::GetAction(OtherActor);
		if ( ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag) )
		{
			MovementComp->Velocity *= -1.f;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if ( USGameplayFunctionLibrary::ApplyDirectionalDamage(InstigatorActor,OtherActor,DamageAmount,SweepResult) )
		{
			if (ActionComponent && HasAuthority())
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	
		Destroy();
		
	}
}



