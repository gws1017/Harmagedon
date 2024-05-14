#include "ProjectHGameModeBase.h"
#include "System/Sound/SoundManager.h"

void AProjectHGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	auto SM = ASoundManager::GetSoundManager();
	SM->SetBGM(EBGMType::EBGMType_InGame);
	SM->PlayBGM();
}
