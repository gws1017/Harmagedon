// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class PROJECTH_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyController();

public:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void RunAI();

public:

	UFUNCTION(BlueprintGetter)
		bool IsUseBehaviorTree() { return bUseBehaviorTree; }
	UFUNCTION(BlueprintCallable)
		void UseBehaviorTree() { bUseBehaviorTree = true; }

	void StopBT();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBlackboardData* BlackboardData;

	bool bUseBehaviorTree = false;
};
