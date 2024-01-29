#include "Actor/SceneTrigger.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

#include "Components/BoxComponent.h"

#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"

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
	GetWorldTimerManager().SetTimer(CutSceneTimer, [&]() {
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
	CLog::Print("Scene End");
}


