#include "Notify/AN_FootStepSound.h"
#include "Material/MaterialSoundList.h"
#include "System/Sound/SoundManager.h"
#include "Global.h"

#include "GameFramework/Character.h"

FString UAN_FootStepSound::GetNotifyName_Implementation() const
{
	return "PlayFootStep";
}

void UAN_FootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(Character);

	FVector Start = Character->GetMesh()->GetSocketLocation(BoneName);
	FVector End = Start;
	End.Z -= TraceDistance;

	//CLog::Print(BoneName.ToString() + " " +  End.ToString());

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Character);

	FHitResult HitResult;
	bool result = UKismetSystemLibrary::LineTraceSingle(
		Character->GetWorld(),
		Start, End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		IgnoreActors,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true);

	if (result)
	{
		EPhysicalSurface SurfaceType = UGameplayStatics::GetSurfaceType(HitResult);
		if (BPSoundList && SurfaceType)
		{
			AMaterialSoundList* SoundList = Cast<AMaterialSoundList>(BPSoundList->GetDefaultObject());
			//재질마다 사운드 채널에 등록된 사운드에셋을 교체하며 재생
			ASoundManager::GetSoundManager()->PlaySFXAtLocation(Character,ESFXType::ESFXType_FootStep,
				Start,SoundList->GetSound(SurfaceType));
		}
	}


}
