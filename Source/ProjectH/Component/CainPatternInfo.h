// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CainPatternInfo.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UCainPatternInfo : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCainPatternInfo();

	// Sets default values for this component's properties
	void Init(TArray<float> InAttackDamage, TArray<FString> InAttackMeans, class UAnimMontage* InBTMontages);

public:
	//공격력, 두번공격 시 추가 공격력
	UPROPERTY(VisibleInstanceOnly, Category = info)
	TArray<float> AttackDamages;

	//첫번째 공격 부위, 두번째..
	UPROPERTY(VisibleInstanceOnly, Category = info)
	TArray<FString> AttackMeans;

	//몽타주
	UPROPERTY(VisibleInstanceOnly, Category = info)
	class UAnimMontage* BTMontage;
};
