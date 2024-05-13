#include "Actor/Objects/EnemySpawner.h"
#include "Actor/Character/Enemy.h"
#include "Global.h"

#include "Components/BoxComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	UHelpers::CreateComponent<UBoxComponent>(this, &SpawnBox, "SpawnBox", GetRootComponent());

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnMonster();
}

void AEnemySpawner::SpawnMonster()
{
	FActorSpawnParameters param;
	verifyf(EnemyClass, L"Enmey class invalid");
	AEnemy* actor = GetWorld()->SpawnActor<AEnemy>(EnemyClass, GetActorLocation(), GetActorRotation(), param);
	if (actor)
	{
		actor->DecrementEnemyFunc.BindUFunction(this, FName("DecrementEnemyCount"));
		MonsterCount++;
	}
}

void AEnemySpawner::RespawnMonster()
{
	if (MonsterCount == 0)
	{
		CLog::Print("Respawn");
		SpawnMonster();
	}
}

