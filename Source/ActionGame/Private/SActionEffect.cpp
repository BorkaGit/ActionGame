// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"


USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

float USActionEffect::GetTimeRemaining() const
{
	if (const AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>() )
	{
		return TimeStarted + Duration - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if ( Duration > 0.0f )
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ThisClass::StopAction, Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if ( Period > 0.0f )
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &ThisClass::ExecutePeriodicEffect, Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	if (USActionComponent* OwningComponent = GetOwningComponent())
	{
		OwningComponent->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

