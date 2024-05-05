// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;
class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONGAME_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;
	 
	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags") 
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;
	
public:

	void Initialize(USActionComponent* NewActionComp);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	FORCEINLINE bool IsAutoStart() const { return bAutoStart; }
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	FORCEINLINE bool IsRunning() const { return RepData.bIsRunning; }

	FORCEINLINE virtual bool IsSupportedForNetworking() const override { return true; }
	
	UFUNCTION()
	void OnRep_RepData();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	UPROPERTY(Replicated)
	float TimeStarted = 0.0f;

private:
	
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Transient, Replicated)
	USActionComponent* ActionComp;
};
