// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/BossGimmick.h"
#include "Components/BoxComponent.h"

// Sets default values
ABossGimmick::ABossGimmick()
{
	//// 트리거 생성 및 설정
	//StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	//StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
	//RootComponent = StageTrigger;
	//StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	////// 트리거에 충돌 종류 설정
	////StageTrigger->SetCollisionProfileName(CPROFILE_PDTRIGGER);
	////// 오버랩 델리게이트에 기믹 함수 등록
	////StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &APdStageGimmick::OnStageTriggerBeginOverlap);
}

void ABossGimmick::OnConstruction(const FTransform& Transform)
{
}

void ABossGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

