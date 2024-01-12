#include "Actor/Character/PlayerAnimInstance.h"
#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(PlayerCharacter);

	
	WeaponEquipped = PlayerCharacter->GetWeaponEquipped();
	
	bIsRolling = PlayerCharacter->IsRolling();
	if (PlayerCharacter->GetVelocity().Length() > 0 && bIsRolling == false)
		bIsMoving = true;
	else
		bIsMoving = false;
}