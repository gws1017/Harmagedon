// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Controller/EnemyController.h"
#include "CainController.generated.h"

 /**************************************************************************************************
 * ���� AI ��Ʈ�ѷ� (ī�ο��� ����)
 *
 * @author	������
 * @date	2024-05-03
 **************************************************************************************************/
UCLASS()
class PROJECTH_API ACainController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	ACainController();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
