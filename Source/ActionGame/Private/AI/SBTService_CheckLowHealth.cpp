// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComponent))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				auto AICharacter = Cast<ASAICharacter>(AIPawn);
				if (ensure(AICharacter))
				{
					BlackBoardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName,AICharacter->GetAttributeComponent()->GetHealth() / AICharacter->GetAttributeComponent()->GetHealthMax() * 100.0f <= AICharacter->GetLowHealthPercentage() );
				}
			}
		}
	}
}
