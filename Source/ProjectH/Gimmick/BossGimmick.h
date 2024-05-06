// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossGimmick.generated.h"


//DECLARE_DELEGATE(FOnStageChangedDelegate);
//USTRUCT(BlueprintType)
//struct FStageChangedDelegateWrapper
//{
//	GENERATED_BODY()
//	FStageChangedDelegateWrapper() { }
//	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
//	FOnStageChangedDelegate StageDelegate;
//};
//
//UENUM(BlueprintType)
//enum class EStageState : uint8
//{
//	READY = 0,
//	FIGHT,
//	REWARD,
//	NEXT
//};

UCLASS()
class PROJECTH_API ABossGimmick : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	ABossGimmick();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

// 스테이지
protected:
	//UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UStaticMeshComponent> Stage;

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

//// 게이트
//protected:
//	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
//	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;
//
//	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
//	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;
//
//	UFUNCTION()
//	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	void OpenAllGates();
//	void CloseAllGates();
//
//// 상태
//protected:
//	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
//	EStageState CurrentState;
//
//	void SetState(EStageState InNewState);
//
//	UPROPERTY()
//	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;
//
//	void SetReady();
//	void SetFight();
//	void SetChooseReward();
//	void SetChooseNext();
//
//// 대전
//protected:
//	// 지정한 클래스에서 상속받은 클래스만 지정가능
//	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
//	TSubclassOf<class ABossMonster> OpponentClass;
//
//	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
//	float OpponentSpawnTime;
//
//	UFUNCTION()
//	void OnOpponentDestroyed(AActor* DestroyedActor);
//
//	FTimerHandle OpponentTimerHandle;
//	void OnOpponentSpawn();

//// 보상
//protected:
//	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
//	TSubclassOf<class APdItemBox> RewardBoxClass;
//
//	// 약참조 : 현재 클래스와 무관한 내부 동작이 있을 때 사용
//	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
//	TArray<TWeakObjectPtr<class APdItemBox>> RewardBoxes;
//
//	TMap<FName, FVector> RewardBoxLocations;
//
//	UFUNCTION()
//	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	void SpawnRewardBoxes();
};
