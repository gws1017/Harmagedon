#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_DecrementStamina.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAN_DecrementStamina : public UAnimNotify
{
	GENERATED_BODY()

public:

	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
