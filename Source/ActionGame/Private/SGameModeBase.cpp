// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SPowerUpBase.h"
#include "SSaveGame.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

class ASPowerUpBase;
DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All);

static TAutoConsoleVariable CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);
static TAutoConsoleVariable CVarSpawnPickups(TEXT("su.SpawnPickups"), true, TEXT("Enable spawning of pickups via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnBotTimerInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPickups, this, &ASGameModeBase::SpawnPickupsTimerElapsed, SpawnPickupsTimerInterval, true);
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if ( ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>() )
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
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

	if ( !Locations.IsEmpty() && MonsterTable )
	{	
		TArray<FMonsterInfoRow*> Rows;
		MonsterTable->GetAllRows("", Rows);
		
		const int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
		const USMonsterData* MonsterData = Rows[RandomIndex]->MonsterData;

		if ( AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, Locations[0], FRotator::ZeroRotator) )
		{
			if ( USActionComponent* ActionComp = USActionComponent::GetAction(NewBot) )
			{
				for  (const auto ActionClass : MonsterData->Actions )
				{
					ActionComp->AddAction(NewBot, ActionClass);
				}
			}
		}
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

void ASGameModeBase::WriteSaveGame()
{
	for ( int32 i = 0; i < GameState->PlayerArray.Num(); ++i )
	{
		if (ASPlayerState* PlayerState = Cast<ASPlayerState>(GameState->PlayerArray[i]))
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // single player only for now
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for ( FActorIterator It(GetWorld()); It; ++It )
	{
		AActor* Actor = *It;
		if (Actor->Implements<USGameplayInterface>())
		{
			FActorSaveData ActorSaveData;
			ActorSaveData.ActorName = Actor->GetName();
			ActorSaveData.Transform = Actor->GetActorTransform();

			FMemoryWriter MemWriter(ActorSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			// Find only variables with UPROPERTY(SaveGame)
			Ar.ArIsSaveGame = true;
			// Converts the actor to a byte array
			Actor->Serialize(Ar);
			
			CurrentSaveGame->SavedActors.Add(ActorSaveData);
		}
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	
}

void ASGameModeBase::LoadSaveGame()
{
	if ( UGameplayStatics::DoesSaveGameExist(SlotName, 0) )
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save game data!"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data"));

		for ( FActorIterator It(GetWorld()); It; ++It )
		{
			AActor* Actor = *It;
			if (Actor->Implements<USGameplayInterface>())
			{
				for ( FActorSaveData ActorData : CurrentSaveGame->SavedActors )
				{
					if ( ActorData.ActorName == Actor->GetName() )
					{
						Actor->SetActorTransform(ActorData.Transform);

						FMemoryReader MemoryReader(ActorData.ByteData);
						FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
						Ar.ArIsSaveGame = true;
						Actor->Serialize(Ar);

						ISGameplayInterface::Execute_OnActorLoaded(Actor);
						
						break;
					}
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		
		UE_LOG(LogTemp, Warning, TEXT("Created New SaveGame Data"));
	}
}