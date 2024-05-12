#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

class UAudioComponent;
class USoundAttenuation;
class USoundCue;

UENUM(BlueprintType)
enum class EBGMType : uint8
{
	EBGMType_None UMETA(DisplayName = "None"),
	EBGMType_Lobby UMETA(DisplayName = "Lobby"),
	EBGMType_InGame UMETA(DisplayName = "InGame"),
	ESFXType_DefaultMax UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ESFXType : uint8
{
	ESFXType_None UMETA(DisplayName = "None"),
	ESFXType_FootStep UMETA(DisplayName = "FootStep"),
	ESFXType_Guard UMETA(DisplayName = "Guard"),
	ESFXType_ParrySwing UMETA(DisplayName = "ParrySwing"),
	ESFXType_ParrySucc UMETA(DisplayName = "ParrySucc"),
	ESFXType_IronToMeat UMETA(DisplayName = "IronToMeat"),
	ESFXType_IronToMeat2 UMETA(DisplayName = "IronToMeat2"),
	ESFXType_DefaultMax UMETA(DisplayName = "Max")
};

UCLASS()
class PROJECTH_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoundManager();

public:
	UFUNCTION(BlueprintPure)
		static ASoundManager* GetSoundManager() {return SoundManagerInstance;}

	UFUNCTION(BlueprintCallable)
		void SetBGM(EBGMType Type);
	UFUNCTION(BlueprintCallable)
		void PlayBGM();

	//소리감쇠를 적용하지않고 어디에서든 들리도록 재생하는 함수
	//주로 UI효과음에서 사용합니다.
	UFUNCTION(BlueprintCallable)
		void PlaySFX2D(ESFXType Type);
	//위치에 따른 소리감쇠를 적용해 재생하는 함수
	//재생하는 액터, 효과음 종류, 재생 위치를 파라미터로 사용합니다.
	UFUNCTION(BlueprintCallable)
		void PlaySFXAtLocation(AActor* PlayActor, ESFXType Type, FVector Location = FVector::ZeroVector, USoundCue* Sound = nullptr);

	UFUNCTION(BlueprintPure)
		FORCEINLINE	 float GetBGMVolume() const { return BGMVolume; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE	 float GetSFXVolume() const { return SFXVolume; }
	UFUNCTION(BlueprintCallable)
		void SetBGMVolume(const float Volume);
	UFUNCTION(BlueprintCallable)
		void SetSFXVolume(const float Volume);

	UFUNCTION(BlueprintCallable)
		void AttachSFXChannel(AActor* AttachActor, ESFXType Type);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USceneComponent* SceneComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UAudioComponent* MainAudio;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UAudioComponent* SFXAudio;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		 USoundAttenuation* AttenuationSettings;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* MainBGM;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		float BGMVolume;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		float SFXVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		TMap<EBGMType, USoundCue*> BGMSoundMap;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		TMap<ESFXType, USoundCue*> SFXSoundMap;
	//액터이름으로 채널을 가져올것
	UPROPERTY(VisibleAnywhere, meta = (Tooltip = "AutoCreate"), Category = "Sound")
		TMap<FString, UAudioComponent*> SFXChannelMap;

	static ASoundManager* SoundManagerInstance;
};
