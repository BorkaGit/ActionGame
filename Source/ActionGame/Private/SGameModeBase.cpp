// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPowerUpBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"

class ASPowerUpBase;
DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All);

static TAutoConsoleVariable CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable CVarSpawnPickups(TEXT("su.SpawnPickups"), true, TEXT("Enable spawning of pickups via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnBotTimerInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPickups, this, &ASGameModeBase::SpawnPickupsTimerElapsed, SpawnPickupsTimerInterval, true);
}

void ASGameModeBase::KillAllBots()
{
	for ( TActorIterator<ASAICharacter> It(GetWorld()); It; ++It )
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = Bot->GetAttributeComponent(); 
		
		if ( AttributeComponent && AttributeComponent->IsAlive() )
		{
			AttributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if ( !CVarSpawnBots.GetValueOnGameThread() )
	{
		UE_LOG(LogGameModeBase, Log, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NumberOfAliveBots = 0;
	for ( TActorIterator<ASAICharacter> It(GetWorld()); It; ++It )
	{
		ASAICharacter* Bot = *It;

		const USAttributeComponent* AttributeComponent = Bot->GetAttributeComponent(); 
		
		if ( AttributeComponent && AttributeComponent->IsAlive() )
		{
			++NumberOfAliveBots;
		}
	}

	if ( DifficultyCurve )
	{
		MaxBotCount = static_cast<int32>(DifficultyCurve->GetFloatValue( GetWorld()->GetTimeSeconds()));
	}

	
	if ( NumberOfAliveBots >= MaxBotCount )
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if ( QueryInstance )
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::SpawnPickupsTimerElapsed()
{
	if ( !CVarSpawnPickups.GetValueOnGameThread() )
	{
		UE_LOG(LogGameModeBase, Log, TEXT("Pickup spawning disabled via cvar 'CVarSpawnPickups'."));
		return;
	}

	int32 NumberOfAlivePickups = 0;
	
	for ( TActorIterator<ASPowerUpBase> It(GetWorld()); It; ++It )
	{
		++NumberOfAlivePickups;
	}

	if (NumberOfAlivePickups >= MaxPickupsCount )
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if ( QueryInstance )
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPickupSpawnQueryCompleted);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if ( ensure(Controller) )
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if ( Player )
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ASGameModeBase::RespawnPlayerElapsed, Player->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, PlayerRespawnTimer, false);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if ( QueryStatus != EEnvQueryStatus::Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"))
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if ( !Locations.IsEmpty() )
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnPickupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if ( QueryStatus != EEnvQueryStatus::Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn pickup EQS Query Failed!"))
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if ( !Locations.IsEmpty() )
	{
		GetWorld()->SpawnActor<AActor>(PickupClasses[FMath::RandRange(0, PickupClasses.Num() - 1)], Locations[0], FRotator::ZeroRotator);
	}
}
