// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/PickupItem.h"
#include "ExpItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API AExpItem : public APickupItem
{
	GENERATED_BODY()
	
public:
	void Init(int32 LostExp, const FVector& Location);

	virtual void OnInteraction() override;

private:

	UPROPERTY(EditAnywhere, Category = "Exp")
		int32 Exp;
	UPROPERTY(EditAnywhere, Category = "Exp")
		float HeightOffset;
};