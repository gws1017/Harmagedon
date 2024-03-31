#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ParryEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAN_ParryEnd : public UAnimNotify
{
	GENERATED_BODY()
public:

	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
