#include "Actor/Character/InventoryCharacter.h"
#include "Component/InventoryComponent.h"
#include "Global.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"

AInventoryCharacter::AInventoryCharacter()
{
	SetInventoryCharacter();

	UHelpers::CreateComponent<USpotLightComponent>(this, &SpotLight, "SpotLight", SpringArm);
	UHelpers::CreateComponent<USceneCaptureComponent2D>(this, &SceneCapture, "SceneCapture", GetCapsuleComponent());
	
	//InitializeArmorComponent();
}

void AInventoryCharacter::BeginPlay()
{
	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(PlayerRef);
	PlayerRef->GetInventory()->SetInventoryPawn(this);
	AttachArmorSocket();

	//인벤토리 방어구 캡처
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