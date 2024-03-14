// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;
class USoundBase;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONGAME_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	ASMagicProjectile();

protected:
	
	
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* CastParticle = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FlightSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);
	
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
};
