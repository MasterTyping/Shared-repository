// Fill out your copyright notice in the Description page of Project Settings.

#include "HBTService_Detect.h"
#include "HMonsterAIController.h"
#include "HMonster.h"
#include "projectlevelCharacter.h"
#include "SwordCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UHBTService_Detect::UHBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UHBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// Player Target ÇßÀ»½Ã.
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AprojectlevelCharacter* APlayer = Cast< AprojectlevelCharacter>(OverlapResult.GetActor());
			//ASwordCharacter* APlayer = Cast< ASwordCharacter>(OverlapResult.GetActor());
			if (APlayer && APlayer->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AHMonsterAIController::TargetKey, APlayer);

				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, APlayer->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);

				DrawDebugLine(World, ControllingPawn->GetActorLocation(), APlayer->GetActorLocation(), FColor::Blue, false, 0.2f);

				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AHMonsterAIController::TargetKey, nullptr);
	}
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
