// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class ASPowerUpBase;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class ACTIONGAME_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnPickups;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotTimerInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float PlayerRespawnTimer = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnPickupsQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float SpawnPickupsTimerInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 MaxPickupsCount = 3;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TArray<TSubclassOf<ASPowerUpBase>> PickupClasses;
	
	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPickupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	 
	void SpawnBotTimerElapsed();

	void SpawnPickupsTimerElapsed();
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);
	
	ASGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllBots();
};
