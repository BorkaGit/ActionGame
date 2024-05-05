// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SWorldUserWidget.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Internal Component."), ECVF_Cheat);


USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();
	

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End  = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	//	FHitResult Hit;
	//	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,Shape );

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint,TraceRadius,32,LineColor,false,2.0f );
		}
		
		if ( AActor* HitActor = Hit.GetActor() )
		{
			if (HitActor->GetClass()->ImplementsInterface(USGameplayInterface::StaticClass()))
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (!DefaultWidgetInstance && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->SetAttachedActor(FocusedActor);

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,End,LineColor, false, 2.0f, 0, 2.0f);
	}
}


void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor );
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No Focused Actor to interact"));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}


