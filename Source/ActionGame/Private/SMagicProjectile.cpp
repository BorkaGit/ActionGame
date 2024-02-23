// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ASMagicProjectile::ASMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageAmount = 20.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		AudioComponent->Stop();
		AudioComponent->SetSound(ImpactSound);
		AudioComponent->Play();

		UGameplayStatics::PlayWorldCameraShake(GetWorld(),ImpactCameraShake,GetActorLocation(),100.0f,1000.0f);
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-DamageAmount);

			Destroy();
		}
	}
}


void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	AudioComponent->SetSound(FlightSound);
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

void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

