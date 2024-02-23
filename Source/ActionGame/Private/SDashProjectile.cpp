// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ASDashProjectile::Explode, ExplodeDelay);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnActorHit);
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
		Explode();
	}
}

void ASDashProjectile::Explode()
{
	MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TeleportEffect,GetActorLocation(),GetActorRotation());
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ASDashProjectile::Teleport, TeleportDelay);
}

void ASDashProjectile::Teleport()
{
	GetInstigator()->SetActorLocation(GetActorLocation());
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}
}

void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
