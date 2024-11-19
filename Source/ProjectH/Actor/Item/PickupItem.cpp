#include "Actor/Item/PickupItem.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Global.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

#include "NavigationSystem.h"

APickupItem::APickupItem()
	:SearchExtent(500.f,500.f,500.f)
{
	UHelpers::CreateComponent<USphereComponent>(this, &OverlapSphere, "OverlapSphere", GetRootComponent());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "StaticMesh", OverlapSphere);
	UHelpers::CreateComponent<UNiagaraComponent>(this, &ItemEffect, "ItemEffect", OverlapSphere);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OverlapSphereBeginOverlap);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OverlapSphereEndOverlap);
}

void APickupItem::OverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		PlayerInstance = OverlapPlayer;
		PlayerInstance->SetOverlappingActor(this);
		//UI���Ÿ� ���⿡
		ToggleOverlapUI(OverlapPlayer->GetPlayerController());
	}
}

void APickupItem::OverlapSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* OverlapPlayer = Cast<APlayerCharacter>(OtherActor);

	if (OverlapPlayer)
	{
		CheckNull(PlayerInstance);
		ToggleOverlapUI(OverlapPlayer->GetPlayerController());
		PlayerInstance->SetOverlappingActor(nullptr);
		PlayerInstance = nullptr;
	}
}

void APickupItem::OnInteraction()
{
	if (ItemEffect)
		ItemEffect->Deactivate();
	Destroy();
}

void APickupItem::ToggleOverlapUI(ABasicPlayerController* PlayerController)
{
	PlayerController->ToggleOverlapUI(OverlapText);
}

void APickupItem::InitializeSpawnLocation(const FVector& Location)
{
	//���������� ��ġ���� Ȯ���Ѵ�
	FNavLocation ValidNavLocation;
	FVector ValidLocation = Location;
	//���� ���忡 ���Ե� �׺���̼� �ý����� ���۷����� �����´�.
	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		//���� ����� ��ȿ�� ��ġ�� ã�´�
		bool bFoundLocation = NavSystem->ProjectPointToNavigation(
			Location,
			ValidNavLocation,
			SearchExtent
		);
		if (bFoundLocation)
			ValidLocation = ValidNavLocation.Location;
		else
			CLog::Print("Invalid Spawn Location");

	}
	DrawDebugSphere(GetWorld(), ValidLocation, 30.f, 12, FColor::White, true, 3.f, 0, 1.f);

	SetActorLocation(ValidLocation);
}
