#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_CollisionEnable.generated.h"

enum class EEquipType : uint8;

UCLASS()
class PROJECTH_API UANS_CollisionEnable : public UAnimNotifyState
{
	GENERATED_BODY()
		
public:
	//에디터에서 노티파이 설정시 무기 유형을 정해준다, 기본값은 오른손 무기
	//노티파이 주체는 플레이어지만, 무기 정보를 얻을 수 있음
	UPROPERTY(EditAnywhere)
	EEquipType WeaponEquipType;

public:

	FString GetNotifyName_Implementation() const;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
