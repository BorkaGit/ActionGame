// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"


UCLASS()
class ACTIONGAME_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void Explode();

	UFUNCTION()
	void Teleport();

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> TeleportEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float ExplodeDelay = 0.2f;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	float TeleportDelay = 0.2f;
	
public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	
	FTimerHandle ExplodeTimerHandle;
	FTimerHandle TeleportTimerHandle;
};
