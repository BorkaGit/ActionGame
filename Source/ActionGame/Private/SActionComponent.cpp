// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"
#include "ActionGame/ActionGame.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_ACTION);

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (const TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<USAction*> ActionsCopy = Actions;
	for ( USAction* Action : ActionsCopy )
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
//	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, (TEXT("%s : %s"), *GetNameSafe(GetOwner()), ActiveGameplayTags.ToStringSimple()));

	/*for (const USAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString:: Printf(TEXT("[%s Action : %s : IsRunning: %s : Outer: %s]"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("True") : TEXT("False"),
			*GetNameSafe(Action->GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}*/
}

USActionComponent* USActionComponent::GetAction(AActor* FromActor)
{
	if ( FromActor )
	{
		return Cast<USActionComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if ( !GetOwner()->HasAuthority() )
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"),*GetNameSafe(ActionClass));
		return;
	}
	
	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if (NewAction->IsAutoStart() && NewAction->CanStart(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ActionToRemove || ActionToRemove->IsRunning())
	{
		return;
	}
	Actions.RemoveSingle(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	
	for (USAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			if (Action->CanStart(Instigator))
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStartAction(Instigator,ActionName);
				}
				
				Action->StartAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator,ActionName);
			}
			
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::HasAction(TSubclassOf<USAction> ActionClass) const
{
	for (const auto Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			return true;
		}
	}
	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}
