// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BossGimmick.h"
#include "Components/BoxComponent.h"

// Sets default values
ABossGimmick::ABossGimmick()
{
	//// Ʈ���� ���� �� ����
	//StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	//StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
	//RootComponent = StageTrigger;
	//StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	////// Ʈ���ſ� �浹 ���� ����
	////StageTrigger->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	////// ������ ��������Ʈ�� ��� �Լ� ���
	////StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &APdStageGimmick::OnStageTriggerBeginOverlap);
}

void ABossGimmick::OnConstruction(const FTransform& Transform)
{
}

void ABossGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

