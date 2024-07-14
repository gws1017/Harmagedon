#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_IncrementHealth.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAN_IncrementHealth : public UAnimNotify
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere)
		float IncrementRate;

public:


	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
