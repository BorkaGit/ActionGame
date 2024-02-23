// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if ( QueryInstance )
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if ( QueryStatus != EEnvQueryStatus::Success )
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"))
		return;
	}

	float NumberOfAliveBots = 0;
	for ( TActorIterator<ASAICharacter> It(GetWorld()); It; ++It )
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = Bot->GetAttributeComponent(); 
		
		if ( AttributeComponent && AttributeComponent->IsAlive() )
		{
			++NumberOfAliveBots;
		}
	}

	float MaxBotCount = 10;

	if ( DifficultyCurve )
	{
		MaxBotCount = DifficultyCurve->GetFloatValue( GetWorld()->GetTimeSeconds() );
	}

	
	if ( NumberOfAliveBots >= MaxBotCount )
	{
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if ( !Locations.IsEmpty() )
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
