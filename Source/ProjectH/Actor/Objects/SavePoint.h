#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "SavePoint.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class PROJECTH_API ASavePoint : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ASavePoint();

protected:
	virtual void BeginPlay() override;



//레벨 업기능 미사용
//public:
//
//	UPROPERTY(BlueprintReadOnly, Category = "UI")
//		class ULevelUpUI* LevelUpUI;
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
//		class TSubclassOf<ULevelUpUI> LevelUpUIClass;
//
//	bool bVisibleLevelUpUI = false;

public:

	UFUNCTION()
		void OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnInteraction() override;
	virtual void ToggleOverlapUI(ABasicPlayerController* PlayerController);


private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UBoxComponent* OverlapBox;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* BallMesh;	
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent*	WaterMesh;
	UPROPERTY(EditDefaultsOnly)
		UNiagaraComponent* ItemEffect;

		UPROPERTY(EditAnywhere)
		FText OverlapText;
};
