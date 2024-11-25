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
	//���ݷ�, �ι����� �� �߰� ���ݷ�
	UPROPERTY(VisibleInstanceOnly, Category = info)
	TArray<float> AttackDamages;

	//ù��° ���� ����, �ι�°..
	UPROPERTY(VisibleInstanceOnly, Category = info)
	TArray<FString> AttackMeans;

	//��Ÿ��
	UPROPERTY(VisibleInstanceOnly, Category = info)
	class UAnimMontage* BTMontage;
};
