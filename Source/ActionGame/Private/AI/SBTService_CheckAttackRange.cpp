// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and target actor

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		if (const AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor")))
		{
			if (const AAIController* MyController = OwnerComp.GetAIOwner(); ensure(MyController))
			{
				if (const APawn* AIPawn = MyController->GetPawn(); ensure(AIPawn))
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					const bool bWithinRange = DistanceTo <= 2000.0f;
					
					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
					
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS );
				}
			}
		}
	}
}