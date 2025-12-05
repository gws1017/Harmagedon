#include "Actor/Character/InventoryCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"

AInventoryCharacter::AInventoryCharacter()
{
	UHelpers::CreateComponent<USpotLightComponent>(this, &SpotLight, "SpotLight", SpringArm);
	UHelpers::CreateComponent<USceneCaptureComponent2D>(this, &SceneCapture, "SceneCapture", GetCapsuleComponent());
}
void AInventoryCharacter::Tick(float DeltaTime)
{
}
void AInventoryCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	CLog::Log("BeginPlay");

	SetInventoryCharacter();

	//SceneCapture Setting
	// FOV : 35
	// CaptureSource : BaseColor
	// Position : (X=-185.679722,Y=0.000000,Z=10.739526)
	// Rotation : (Pitch=-5.000000,Yaw=0.000000,Roll=0.000000)


	USkeletalMeshComponent* SMesh = GetMesh();
	if (SMesh)
	{
		SMesh->VisibilityBasedAnimTickOption =
			EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	};

	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(PlayerRef);
	PlayerRef->GetInventory()->SetInventoryPawn(this);
	AttachArmorSocket();

	//인벤토리 방어구 캡처
	
	SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList; //등록된 것만 캡처
	SceneCapture->ShowOnlyComponent(GetMesh());
	for (auto [Type, ArmorArray] : ArmorComponents)
	{
		for (auto ArmorComponent : ArmorArray.ArmorArray)
			SceneCapture->ShowOnlyComponent(ArmorComponent);
	}
	
}

void AInventoryCharacter::SetCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->ShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}

void AInventoryCharacter::RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->RemoveShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}