#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UnEquip.generated.h"

enum class EEquipType : uint8;

UCLASS()
class PROJECTH_API UANS_UnEquip : public UAnimNotifyState
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere)
		EEquipType WeaponEquipType;

public:
	FString GetNotifyName_Implementation() const;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
