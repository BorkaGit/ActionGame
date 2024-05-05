// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"


class USizeBox;

UCLASS()
class ACTIONGAME_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	AActor* GetAttachedActor() const { return AttachedActor; }
	FVector GetWorldOffset() const { return WorldOffset; }
	
	void SetAttachedActor(AActor* NewActor) { AttachedActor = NewActor; }
	void SetWorldOffset(const FVector& NewOffset) { WorldOffset = NewOffset; }
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;
};
