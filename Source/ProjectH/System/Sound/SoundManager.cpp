#include "System/Sound/SoundManager.h"
#include "Global.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundCue.h"
#include "SoundManager.h"

ASoundManager* ASoundManager::SoundManagerInstance = nullptr;

ASoundManager::ASoundManager()
	:BGMVolume(0.1f),
	SFXVolume(0.3f)
{
	ASoundManager::SoundManagerInstance = this;
	UHelpers::CreateComponent(this, &SceneComponent, "SceneComponent", GetRootComponent());
	UHelpers::CreateComponent(this, &MainAudio, "MainAudio", SceneComponent);
	UHelpers::CreateComponent(this, &SFXAudio, "SFXAudio", SceneComponent);
	UHelpers::CreateActorComponent(this, &AttenuationSettings, "AttenuationSettings");
	MainAudio->SetVolumeMultiplier(BGMVolume);
	SFXAudio->SetVolumeMultiplier(SFXVolume);
}

void ASoundManager::SetBGM(EBGMType Type)
{
	if (IsValid(this) == false) return;
	if (BGMSoundMap.IsEmpty() == false)
	{
		if (BGMSoundMap[Type] == nullptr)
		{
			CLog::Log("Invalid BGM Type!");
			return;
		}
		MainBGM = BGMSoundMap[Type];

	}
}

void ASoundManager::PlayBGM()
{
	CheckNull(SoundManagerInstance);
	if (IsValid(SoundManagerInstance) == false) return;
	CheckNull(MainAudio);
	if (IsValid(MainAudio) == false) return;
	if (MainBGM)
	{
		MainAudio->SetSound(MainBGM);
		MainAudio->Play();
	}
	else
		CLog::Log("Invalid BGM Asset");

}

void ASoundManager::PlaySFX2D(ESFXType Type)
{
	CheckNull(SoundManagerInstance);

	if ( !SFXSoundMap.IsEmpty())
	{
		if (SFXSoundMap.Contains(Type) == false)
		{
			CLog::Log("Invalid SFX Type!");
			return;
		}

		SFXAudio->SetSound(SFXSoundMap[Type]);
		SFXAudio->Play();
	}
}

void ASoundManager::PlaySFXAtLocation(AActor* PlayActor, ESFXType Type, FVector Location, USoundCue* Sound)
{
	CheckNull(SoundManagerInstance);

	if (SFXSoundMap.IsEmpty() == false)
	{
		if (SFXSoundMap.Contains(Type) == false)
		{
			CLog::Log("This Sound doesn't exist");
			return;
		}
		//액터당 채널 하나를 사용한다, 한액터에서 두가지소리를 동시에내는경우는 드물다
		//걷기 사운드를 내면서 점프사운드내기? -> 실제로 일어나지 않는 일
		//재생할 액터에 채널이 달려있나 확인
		if (SFXChannelMap.Contains(PlayActor->GetName()) == false)
			AttachSFXChannel(PlayActor, Type);
		
		//미리 지정한 사운드가 있으면 그걸로 교체후 재생
		if (!!Sound)
			SFXChannelMap[PlayActor->GetName()]->SetSound(Sound);
		else
			SFXChannelMap[PlayActor->GetName()]->SetSound(SFXSoundMap[Type]);

		SFXChannelMap[PlayActor->GetName()]->Play();
	}

}

void ASoundManager::SetBGMVolume(const float Volume)
{
	MainAudio->SetVolumeMultiplier(BGMVolume);
}

void ASoundManager::SetSFXVolume(const float Volume)
{
	SFXAudio->SetVolumeMultiplier(SFXVolume);
}

void ASoundManager::AttachSFXChannel(AActor* AttachActor, ESFXType Type)
{
	FName Name = FName("SFxChannel" + StaticCast<int32>(Type));
	UAudioComponent* SFXChannel = NewObject<UAudioComponent>(AttachActor, UAudioComponent::StaticClass(), Name);

	if (SFXChannelMap.Contains(AttachActor->GetName()) == false)
	{
		SFXChannelMap.Add(AttachActor->GetName(), SFXChannel);
		SFXChannel->SetSound(SFXSoundMap[Type]);
		SFXChannel->AttenuationSettings = AttenuationSettings;
		SFXChannel->RegisterComponent();
		SFXChannel->AttachToComponent(AttachActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
		CLog::Log("This SFXType already exist");
}

