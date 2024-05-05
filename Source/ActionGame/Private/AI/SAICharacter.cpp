// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


ASAICharacter::ASAICharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));
	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
 		SetTargetActor(Pawn);
		MulticastPawnSeen();
	}
}

void ASAICharacter::MulticastPawnSeen_Implementation()
{
	if (USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(),SpottedWidgetClass))
	{
		NewWidget->SetAttachedActor(this);
		NewWidget->AddToViewport(10);
		
		FTimerHandle TimerHandle;
		auto TimerDelegate = FTimerDelegate::CreateLambda([NewWidget]()
		{
			if (NewWidget)
			{
				NewWidget->RemoveFromParent();
			}
		});
		GetWorldTimerManager().SetTimer(TimerHandle,TimerDelegate, 2.0f,false);
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
	
}

AActor* ASAICharacter::GetTargetActor() const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if ( Delta < 0.0f )
	{
		if ( InstigatorActor != this )
		{
			SetTargetActor(InstigatorActor);
		}

		if ( ActiveHealthBar == nullptr )
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if ( ActiveHealthBar )
			{
				ActiveHealthBar->SetAttachedActor(this); 
				ActiveHealthBar->AddToViewport();
			}
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if ( NewHealth <= 0.0f )
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("IsDead");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10.0f);
		}
	}
}
