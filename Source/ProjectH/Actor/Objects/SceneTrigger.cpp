#include "Actor/Objects/SceneTrigger.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Character/Cain.h"
#include "Actor/Character/Enemy.h"
#include "System/Sound/SoundManager.h"
#include "Global.h"

#include "Components/BoxComponent.h"

#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "System/MySaveGame.h"
#include "System/MyGameInstance.h"

ASceneTrigger::ASceneTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	UHelpers::CreateComponent<UBoxComponent>(this, &TriggerBox, "TriggerBox");
}

void ASceneTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASceneTrigger::OnComponentBeginOverlap);

}

void ASceneTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	CheckNull(player);

	PlayScene(player);

	if (TriggerBox->OnComponentBeginOverlap.IsBound())
		TriggerBox->OnComponentBeginOverlap.Clear();
}

void ASceneTrigger::PlayScene(APlayerCharacter* player)
{
	auto GameInstance = GetGameInstance<UMyGameInstance>();
	if (GameInstance->IsNewGame() == false)
	{
		// Cain보스를 한번 처치했다면 보스 룸에 가도 보스 등장 안함
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->PlayerName, SaveGameInstance->UserIndex));
		if (SaveGameInstance->SaveData.CainDie)
		{
			return;
		}
	}

	CheckNull(CutScene);
	DisableInput(player->GetController<ABasicPlayerController>());

	FMovieSceneSequencePlaybackSettings setting;
	ALevelSequenceActor* OutActor;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CutScene, setting, OutActor);


	TArray<AActor*> StopActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), StopActors);
	
	for (auto actor : StopActors)
	{
		actor->SetActorHiddenInGame(true);
		actor->SetActorTickEnabled(false);
	}

	float EndTime = StaticCast<float>(SequencePlayer->GetEndTime().AsSeconds());

	SequencePlayer->Play();
	GetWorldTimerManager().SetTimer(CutSceneTimer, [this,player, StopActors]() {
		for (auto actor : StopActors)
		{
			actor->SetActorHiddenInGame(false);
			actor->SetActorTickEnabled(true);
		}
		FinishScene(player);
		}, EndTime, false);
}

void ASceneTrigger::FinishScene(APlayerCharacter* player)
{
	//컷신 종료후 처리할 내용
	//CLog::Print("Scene End");

	// 보스 스폰 후 보스 체력 띄우기
	CheckNull(CainBP);
	FActorSpawnParameters params;
	UWorld* WorldRef = player->GetWorld();
	auto CainRef = WorldRef->SpawnActor<ACain>(CainBP, SpawnPosition,FRotator::ZeroRotator, params);
	bPlayerEnterRoom = true;

	ABasicPlayerController* PlayerController = player->GetPlayerController();
	PlayerController->ShowBossHUD(CainRef);

	CainRef->SetActorLocation(SpawnPosition);
	ASoundManager::GetSoundManager()->SetBGM(EBGMType::EBGMType_BossCain);
	ASoundManager::GetSoundManager()->PlayBGM();
}


