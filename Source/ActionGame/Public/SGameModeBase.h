// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class USSaveGame;
class ASPowerUpBase;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UDataTable;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USMonsterData* MonsterData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward = 20.0f;
};

UCLASS()
class ACTIONGAME_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnPickups;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotTimerInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float PlayerRespawnTimer = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnPickupsQuery = nullptr;
	
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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

private:
	
	FString SlotName = "SaveGame01";

	UPROPERTY()
	USSaveGame* CurrentSaveGame = nullptr;
};
