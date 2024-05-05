// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta)							;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorALive(AActor* Actor);
	
	USAttributeComponent();
	
protected:

	virtual void BeginPlay() override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = "0.0", ClampMax = "100.0")) 
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = "0.0")) 
	float HealthMax = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = "0.0", ClampMax = "100.0")) 
	float Rage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes", meta = (ClampMin = "0.0")) 
	float RageMax = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Coef", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float RageCoef = 0.0f;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastRageChanged(float NewRage, float Delta);
	
public:

	FORCEINLINE float GetHealth()		const		{ return Health; }
	FORCEINLINE float GetHealthMax()	const		{ return HealthMax; }
	FORCEINLINE float GetRage()			const		{ return Rage; }
	FORCEINLINE float GetRageMax()		const		{ return RageMax; }
	FORCEINLINE float GetRageCoef()		const		{ return RageCoef; }
	
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFullHealth() const { return Health == HealthMax; }
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyRageChange(float Delta);

};
