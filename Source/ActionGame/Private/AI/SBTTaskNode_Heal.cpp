// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_Heal.h"
#include "SAttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type USBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		APawn* Pawn = MyController->GetPawn();
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Pawn);
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(nullptr, AttributeComp->GetHealthMax() - AttributeComp->GetHealth());
		}
		
	}
	return EBTNodeResult::Failed;
}
