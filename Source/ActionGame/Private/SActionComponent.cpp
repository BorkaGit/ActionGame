// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, (TEXT("%s : %s"), *GetNameSafe(GetOwner()), ActiveGameplayTags.ToStringSimple()));
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

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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
	for (USAction* Action : Actions)
	{
		if (Action->ActionName == ActionName)
		{
			if (Action->CanStart(Instigator))
			{
				Action->StartAction(Instigator);
			}
			return true;
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


