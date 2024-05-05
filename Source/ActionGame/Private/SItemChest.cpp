// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened() const
{
	LidMesh->SetRelativeRotation(FRotator(bLidOpened ? TargetPitch : 0.0f, 0.0f, 0.0f));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}
