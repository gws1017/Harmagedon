// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CainPatternInfo.h"

UCainPatternInfo::UCainPatternInfo()
{
}

// Sets default values for this component's properties
void UCainPatternInfo::Init(TArray<float> InAttackDamages, TArray<FString> InSocketNames, class UAnimMontage* InBTMontage)
{
	AttackDamages = InAttackDamages;
	SocketNames = InSocketNames;
	BTMontage = InBTMontage;
}