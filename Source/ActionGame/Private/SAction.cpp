// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "ActionGame/ActionGame.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this))
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags); 
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if ( GetOwningComponent()->GetOwner()->HasAuthority() )
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	
	GetOwningComponent()->OnActionStarted.Broadcast(ActionComponent, this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	
	GetOwningComponent()->OnActionStopped.Broadcast(ActionComponent, this);
}

UWorld* USAction::GetWorld() const
{
	if (const AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	USActionComponent* ActionComponent = GetOwningComponent();

	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::OnRep_RepData()
{
	if (IsRunning())
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
	DOREPLIFETIME(USAction,TimeStarted)
}
