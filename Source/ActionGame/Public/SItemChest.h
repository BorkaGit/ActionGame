// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONGAME_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:	
	
	ASItemChest();

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.0f;
	
protected:

	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", SaveGame) // RepNotify
	bool bLidOpened = false;

	UFUNCTION()
	void OnRep_LidOpened() const;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

private:
	
	void Interact_Implementation(APawn* InstigatorPawn) override;

	void OnActorLoaded_Implementation() override;

};
