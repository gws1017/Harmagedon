#include "UI/PlayerHUD.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerInstance = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}
