#include "Actor/Objects/SavePoint.h"
#include "Actor/Objects/EnemySpawner.h"
#include "Actor/Character/PlayerCharacter.h"
#include "System/Sound/SoundManager.h"
#include "Global.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = false;
	UHelpers::CreateComponent<UBoxComponent>(this, &OverlapBox, "OverlapBox", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapBox);
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &BallMesh, "BallMesh", OverlapBox);
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &WaterMesh, "WaterMesh", OverlapBox);
	UHelpers::CreateComponent<UNiagaraComponent>(this, &ItemEffect, "Effect", OverlapBox);
}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OverlapBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ASavePoint::OverlapBoxEndOverlap);

}

void ASavePoint::OverlapBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		OverlapPlayer->SetOverlappingActor(this);
	}
}

void ASavePoint::OverlapBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		OverlapPlayer->SetOverlappingActor(nullptr);
	}
}

void ASavePoint::OnInteraction()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ASoundManager::GetSoundManager()->PlaySFXAtLocation(this, ESFXType::ESFXType_Save, GetActorLocation());

	player->SetHP(player->GetMaxHP());
	player->SetStartPoint(player->GetActorLocation());
	TArray<AEnemySpawner*> SpawnerArray;
	UHelpers::FindActors<AEnemySpawner>(GetWorld(), SpawnerArray);
	for (auto spawner : SpawnerArray)
	{
		spawner->RespawnMonster();
	}
}

