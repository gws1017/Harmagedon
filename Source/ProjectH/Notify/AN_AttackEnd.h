#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_AttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UAN_AttackEnd : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	FString GetNotifyName_Implementation() const;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);
};
