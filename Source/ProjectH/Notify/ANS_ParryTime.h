#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ParryTime.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UANS_ParryTime : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		bool bCanParryed;
	UPROPERTY(EditAnywhere)
		bool bCanDamaged;

	UPROPERTY(EditAnywhere)
		float ConsumeStamina;
public:

	FString GetNotifyName_Implementation() const;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
